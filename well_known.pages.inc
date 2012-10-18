<?php

/**
 * .well-known page; Menu callback.
 */
function well_known_page() {
  $link = 'http://tools.ietf.org/html/rfc5785';
  print t('See <a href="!link">!link</a> for more information.', array(
    '!link' => url($link)
  ));
}
