<?php

/**
 *  @file media_ooyala/includes/MediaOoyalaStreamWrapper.inc
 *
 *  Create a Ooyala Stream Wrapper class for the Media/Resource module.
 */

/**
 *  Create an instance like this:
 *  $ooyala = new MediaOoyalaStreamWrapper('ooyala://v/[video-code]');
 */
class MediaOoyalaStreamWrapper extends MediaReadOnlyStreamWrapper {

  // Overrides $base_url defined in MediaReadOnlyStreamWrapper.
  protected $base_url = 'http://player.ooyala.com';


  static function getMimeType($uri, $mapping = NULL) {
    return 'video/ooyala';
  }

  /**
   * Call the ooyala Simple API and fetch the video information.
   *
   * See http://ooyala.com/
   *
   * @return
   *   Array of properties.
   */
  static function getVideoProperties($video_id) {
    // The .php format returns a serialized array.
    $item_video = ooyala_item_load($video_id);
    $response = drupal_http_request($item_video['preview_image_url']);
    if (!isset($response->error)) {
    	return $item_video['preview_image_url'];
    } else {

    }
  }

  function getTarget($f) {
    return FALSE;
  }

  function interpolateUrl() {
    return 'http://ak.c.ooyala.com/' . intval($this->parameters['v']);
  }

  function getOriginalThumbnailPath() {
    $video_properties = self::getVideoProperties($this->parameters['v']);
    return $video_properties;
  }

  function getLocalThumbnailPath() {
    $local_path = 'public://media-ooyala/' . intval($this->parameters['v']) . '.jpg';
    if (!file_exists($local_path)) {
      $dirname = drupal_dirname($local_path);
      file_prepare_directory($dirname, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
      @copy($this->getOriginalThumbnailPath(), $local_path);
    }
    return $local_path;
  }
}
