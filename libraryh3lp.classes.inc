<?php

class libraryh3lp {

  private $username = '';
  private $password = '';
  private $base_url = '';
  private $cookie = '';
  private $ch = NULL;

  public function __construct($username = '', $password = '') {
    if (!$username) {
      $this->username = variable_get('libraryh3lp_username', NULL);
    }
    if (!$password) { 
      $this->password = variable_get('libraryh3lp_password', NULL);
    }
    $this->base_url = variable_get('libraryh3lp_base_url', 'https://us.libraryh3lp.com/2011-12-03');

    if ($this->username == NULL || $this->password == NULL) {
      watchdog('libraryh3lp', t('libraryh3lp username and password not set'));
      return FALSE;
    }
    if (!$this->base_url) {
      return FALSE;
    }
    return TRUE;
  }

  private function set_curl_handle() {
    if (!$this->ch) {
      $this->ch = curl_init();
    }
  }

  private function request($method, $args='', $params = array()) {
    $url = $this->base_url . '/' . $method;
    if ($args) {
      $url .= '/' . $args;
    }
    curl_setopt($this->ch, CURLOPT_URL, $url);
    curl_setopt($this->ch, CURLOPT_COOKIE, $this->cookie);
    curl_setopt($this->ch, CURLOPT_RETURNTRANSFER, 1);
    $return = new stdClass();
    $result = curl_exec($this->ch);
    list($return->headers, $return->body) = explode("\r\n\r\n", $result, 2);
    $return->info = curl_getinfo($this->ch);
    return $return;
  }

  private function post($method, $args='', $params = array()) {
    $this->set_curl_handle();
    if ($this->authenticate()) {
      curl_setopt($this->ch, CURLOPT_HTTPGET, 0);
      $this->set_post_options($params);
      return $this->request($method, $args);
    }
  } 

  private function get($method, $args='', $params = array()) {
    $this->set_curl_handle();
    if ($this->authenticate()) {
      // set options; remove post options
      curl_setopt($this->ch, CURLOPT_HTTPGET, 1);
      curl_setopt($this->ch, CURLOPT_POST, 0);
      return $this->request($method, $args, $params);
    }
  } 

  private function set_post_options($params) {
    curl_setopt($this->ch, CURLOPT_POST, 1);
    curl_setopt($this->ch, CURLOPT_POSTFIELDS, http_build_query($params, '', '&'));
  }

  private function authenticate() {
    if ($this->cookie) {
      return TRUE;
    }
    $this->set_curl_handle();
    $this->set_post_options(array('username' => $this->username, 'password' => $this->password));
    curl_setopt($this->ch, CURLOPT_HEADER, 1);
    $return = $this->request('auth/login');
//    curl_setopt($this->ch, CURLOPT_HEADER, NULL); // unset for later

    if ($return->info['http_code'] != 200) {
      watchdog('libraryh3lp', t('Unable to authenticate. HTTP response: !code', 
                              array('!code' => $return->info['http_code'])));
      return FALSE;
    } 

    $result = json_decode($return->body);
    if (!$result->success) {
      watchdog('libraryh3lp', t('Unable to authenticate. Message: !message', 
                              array('!message' => $result->message)));
      return FALSE;
    }

    // isolate the cookie
    $this->cookie = '';
    preg_match('/^Set-Cookie:\s*([^;]*)/mi', $return->headers, $m);
    if (isset($m)) {
      $this->cookie = $m[1];
    }
    return TRUE;
  }

  public function close() {
    curl_close($this->curl_handle);
  }

  public function users() {
    return json_decode($this->get('users')->body);
  }

  public function queues() {
    return json_decode($this->get('queues')->body);
  } 
}
