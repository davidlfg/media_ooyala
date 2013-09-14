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
}
