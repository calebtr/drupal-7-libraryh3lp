drupal-7-libraryh3lp
====================

A Drupal 7 module to interface with the libraryh3lp/mycustomercloud API at http://dev.mycustomercloud.com/. 

The goal of this module is to allow Drupal site administrators access to the libraryh3lp API with no additional back-end development.


WARNING
=======

This module stores your libraryh3lp administratie password in cleartext in the database. I'm open to a better idea.


Configuration
=============

Download and enable the module. Configure the settings at admin/config/system/libraryh3lp. Configure blocks if you want.


Blocks 
======

  libraryh3lp_queues - list queues, sorted by availability


libraryh3lp SDK-light
=====================

This module includes a lightweight PHP SDK for the libraryh3lp API, in order to help along with Drupal integration. 

If you need a fully-developed SDK, try the official Python SDK at https://github.com/nubgames/libraryh3lp-sdk-python or the Ruby SDK at https://github.com/nubgames/libraryh3lp-sdk-ruby.

  Usage

    $handle = new libraryh3lp(); 

  Supported methods

    queues: returns a listing of queues
    users: returns a listing of users

  Example
    
    $handle = new libraryh3lp();
    $queues = $handle->queues();

