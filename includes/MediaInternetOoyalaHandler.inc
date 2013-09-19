<?php

/**
 * @file media_ooyala/includes/MediaInternetOoyalaHandler.inc
 *
 * Contains MediaInternetOoyalaHandler.
 */

/**
 * Implementation of MediaInternetBaseHandler.
 *
 * @see hook_media_internet_providers().
 */
class MediaInternetOoyalaHandler extends MediaInternetBaseHandler {
  /**
   * Check if a Ooyala video id is valid.
   *
   * Check against the oembed stream instead of the gdata api site to
   * avoid "yt:quota too_many_recent_calls" errors.
   *
   * @return
   *   Boolean.
   */
  static public function validId($embed_code) {
    try {
      $response = drupal_http_request('http://api.ooyala.com/v2/assets/$embed_code/metadata');
      return TRUE;
    } catch (Exception $e) {
      throw new MediaInternetValidationException($e);
    }
  }

  public function parse($embedCode) {
    if (isset($embedCode) && self::validId($embedCode)) {
      return file_stream_wrapper_uri_normalize('ooyala://v/' . $embedCode);
    }
  }

   public function claim($embedCode) {
    if ($this->parse($embedCode)) {
      return TRUE;
    }
  }

  public function getFileObject() {
    $uri = $this->parse($this->embedCode);
    $file = file_uri_to_object($uri, TRUE);
    $file->type = 'video';
    $file->filemime = 'video/ooyala';
    if (empty($file->fid) && $info = $this->getOEmbed()) {
      $file->filename = truncate_utf8($info['name'], 255);
    }
    dpm($file);
    return $file;
  }

  /**
   * Returns information about the media. See http://video.search.yahoo.com/mrss.
   *
   * @return
   *   If ATOM+MRSS information is available, a SimpleXML element containing
   *   ATOM and MRSS elements, as per those respective specifications.
   *
   * @todo Would be better for the return value to be an array rather than a
   *   SimpleXML element, but media_retrieve_xml() needs to be upgraded to
   *   handle namespaces first.
   */
  public function getMRSS() {
    $uri = $this->parse($this->embedCode);
    $video_id = arg(1, file_uri_target($uri));
    $rss_url = url('http://api.ooyala.com/v2/assets/$video_id');
    // @todo Use media_retrieve_xml() once it's upgraded to include elements
    //   from all namespaces, not just the document default namespace.
    $request = drupal_http_request($rss_url);
    if (!isset($request->error)) {
      $entry = simplexml_load_string($request->data);
    }
    else {
      throw new Exception("Error Processing Request. (Error: {$response->code}, {$response->error})");

      //if request wasn't successful, create object for return to avoid errors
      $entry = new SimpleXMLElement();
    }
    return $entry;
  }

  /**
   * Returns information about the media. See http://www.oembed.com/.
   *
   * @return
   *   If oEmbed information is available, an array containing 'title', 'type',
   *   'url', and other information as specified by the oEmbed standard.
   *   Otherwise, NULL.
   */
  public function getOEmbed() {
    $uri = $this->parse($this->embedCode);
    //dpm(ooyala_item_load($embed_code));
    $external_url = file_create_url($uri);
    //dpm($external_url);
    if ($info = $this->getOoyalaLoad($this->embedCode)) {
      return $info;
    } else {
      throw new Exception("Error Processing Request");
      return;
    }
  }
 
  public function getOoyalaLoad($embedCode) {
    module_load_include('inc', 'ooyala', 'includes/ooyala.api');
    if (isset($embedCode) && ooyala_api_available()) {
      return ooyala_item_load($embedCode);
    } else {
      return FALSE;
    }
  }
}
