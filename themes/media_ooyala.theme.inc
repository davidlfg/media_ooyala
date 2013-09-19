<?php

/**
 * @file media_ooyala/themes/media_ooyala.theme.inc
 *
 * Theme and preprocess functions for Media: Ooyala.
 */

/**
 * Preprocess function for theme('media_ooyala_video').
 */
function media_ooyala_preprocess_media_ooyala_video(&$variables) {
  // Build the URI.
  $wrapper = file_stream_wrapper_get_instance_by_uri($variables['uri']);
  $parts = $wrapper->get_parameters();
  $variables['embed_code'] = $parts['v'];

  static $container_ids = array();
  // Ensure that each container ID is unique across requests.
  $container_id = 'ooyala-container-' . REQUEST_TIME;
  // Ensure that each container ID is unique for this request.
  if (in_array($container_id, $container_ids)) {
    $container_id = $container_id . '-' . count($container_ids);
  }
  $container_ids[] = $container_id;
  $variables['container_id'] = $container_id;
  $variables['params'] = $variables['options'];
  $variables['params']['width'] = (int) $variables['params']['width'];
  $variables['params']['height'] = (int) $variables['params']['height'];
}