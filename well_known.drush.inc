<?php

/**
 * Implements hook_drush_command().
 */
function well_known_drush_command() {
  $items = array();
  $items['well-known-install'] = array(
    'description' => 'Setup .well-known folder structure.',
    'arguments' => array(),
    'options' => array(),
    'examples' => array(
      'drush well-known-install' => 'Setup .well-known folder structure.',
      'drush wk-i' => 'Setup .well-known folder structure.',
    ),
    'aliases' => array('wk-i'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );
  $items['well-known-uninstall'] = array(
    'description' => 'Remove .well-known folder structure.',
    'arguments' => array(),
    'options' => array(),
    'examples' => array(
      'drush well-known-uninstall' => 'Remove .well-known folder structure.',
      'drush wk-u' => 'Remove .well-known folder structure.',
    ),
    'aliases' => array('wk-u'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );
  return $items;
}

/**
 * Setup folder structure; Drush command
 */
function drush_well_known_install() {
  $directory = '.well-known';
  if (!file_exists($directory)) {
    mkdir($directory);
  }
  chmod($directory, 0777);
  well_known_create_htaccess($directory);
  chmod($directory, 0555);
}

/**
 * Setup folder structure; Drush command
 */
function drush_well_known_uninstall() {
  $directory = '.well-known';
  if (file_exists($directory)) {
    chmod($directory, 0777);
    $htaccess_path =  $directory . '/.htaccess';
    drupal_unlink($htaccess_path);
    drupal_rmdir($directory);
  }
}

/**
 * Creates a .htaccess file in the given directory.
 */
function well_known_create_htaccess($directory) {
  if (file_uri_scheme($directory)) {
    $directory = file_stream_wrapper_uri_normalize($directory);
  }
  else {
    $directory = rtrim($directory, '/\\');
  }
  $htaccess_path =  $directory . '/.htaccess';

  if (file_exists($htaccess_path)) {
    // Short circuit if the .htaccess file already exists.
    return;
  }

  // Public .htaccess file.
  $htaccess_lines = "Options None\nOptions +FollowSymLinks\n\n<IfModule mod_rewrite.c>\n  RewriteEngine on\n</IfModule>";

  // Write the .htaccess file.
  if (file_put_contents($htaccess_path, $htaccess_lines)) {
    drupal_chmod($htaccess_path, 0444);
  }
  else {
    $variables = array(
      '%directory' => $directory,
      '!htaccess' => '<br />' . nl2br(check_plain($htaccess_lines)),
    );
    watchdog('security', "Security warning: Couldn't write .htaccess file. Please create a .htaccess file in your %directory directory which contains the following lines: <code>!htaccess</code>", $variables, WATCHDOG_ERROR);
  }
}