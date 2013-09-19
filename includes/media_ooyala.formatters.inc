<?php

/**
 * @file
 */

/**
 * Implements hook_file_formatter_info().
 */
function media_ooyala_file_formatter_info() {
  $formatters['media_ooyala_video'] = array(
    'label' => t('Ooyala Video'),
    'file types' => array('video'),
    'default settings' => array(),
    'view callback' => 'media_ooyala_file_formatter_video_view',
    'settings callback' => 'media_ooyala_file_formatter_video_settings',
  );
  $formatters['media_ooyala_video']['default settings'] = array(
    'width' => (int) variable_get('ooyala_video_width', 640),
    'height' => (int) variable_get('ooyala_video_height', 390),
    'ooyalaPlayerId' => variable_get('ooyala_player_id', ''),
    'enableChannels' => TRUE,
    'wmode' => 'transparent', //opaque, transparent, window
    'onCreate' => 'Drupal.ooyala.onCreate',
  );

  return $formatters;
}

/**
 * Implements hook_file_formatter_FORMATTER_view().
 */
function media_ooyala_file_formatter_video_view($file, $display, $langcode) {
  $scheme = file_uri_scheme($file->uri);
  // WYSIWYG does not yet support video inside a running editor instance.
  if ($scheme == 'ooyala' && empty($file->override['wysiwyg'])) {
    $element = array(
      '#theme' => 'media_ooyala_video',
      '#uri' => $file->uri,
      '#options' => array(),
    );
    foreach (array('width', 'height', 'ooyalaPlayerId', 'enableChannels', 'wmode', 'onCreate') as $setting) {
      $element['#options'][$setting] = isset($file->override[$setting]) ? $file->override[$setting] : $display['settings'][$setting];
    }
    return $element;
  }
}

/**
 * Implements hook_file_formatter_FORMATTER_settings().
 */
function media_ooyala_file_formatter_video_settings($form, &$form_state, $settings) {
  $element = array();
  $element['media_ooyala'] = array(
    '#title' => t('Config media ooyala'),
    '#type' => 'container',
    '#suffix' => l(t('Change settings for the player'), 'admin/config/media/ooyala/settings'),
  );
  $element['media_ooyala']['width'] = array(
    '#title' => t('Width'),
    '#type' => 'textfield',
    '#default_value' => $settings['width'],
    '#element_validate' => array('_ooyala_validate_video_width_and_height'),
    '#disabled' => TRUE,
  );
  $element['media_ooyala']['height'] = array(
    '#title' => t('Height'),
    '#type' => 'textfield',
    '#default_value' => $settings['height'],
    '#element_validate' => array('_ooyala_validate_video_width_and_height'),
    '#disabled' => TRUE,
  );
  $element['media_ooyala']['ooyala_player_id'] = array(
    '#title' => t('Ooyala player id'),
    '#type' => 'textfield',
    '#default_value' => $settings['ooyalaPlayerId'],
    '#disabled' => TRUE,
  );

  return $element;
}

/**
 * Validation for width and height.
 */
function _ooyala_validate_video_width_and_height($element, &$form_state, $form) {

  // Check if the value is a number with an optional decimal or percentage sign, or "auto".
  if (!empty($element['#value']) && !preg_match('/^(auto|([0-9]*(\.[0-9]+)?%?))$/', $element['#value'])) {
    form_error($element, t("The value entered for @dimension is invalid. Please insert a unitless integer for pixels, a percent, or \"auto\". Note that percent and auto may not function correctly depending on the browser and doctype.", array('@dimension' => $element['#title'])));
  }
}

/**
 * Implements hook_file_formatter_FORMATTER_settings().
 */
function media_ooyala_file_formatter_image_settings($form, &$form_state, $settings) {
  $element = array();
  $element['image_style'] = array(
    '#title' => t('Image style'),
    '#type' => 'select',
    '#options' => image_style_options(FALSE),
    '#default_value' => $settings['image_style'],
    '#empty_option' => t('None (original image)'),
  );
  return $element;
}

/**
 * Implements hook_file_default_displays().
 */
function media_ooyala_file_default_displays() {

  // Default settings for displaying as a video.
  $default_displays = array();

  $default_video_settings = array(
    'default' => array(
      'width' => 640,
      'height' => 390,
    ),
    'teaser' => array(
      'width' => 560,
      'height' => 340,
      'autoplay' => FALSE,
    ),
    // Legacy view modes included to support older versions of Media,
    // before #1051090 went through. They do no harm and can sit here
    // until there is a Media 2.x stable.
    // @TODO: Remove when Media 2.0 is released.
    'media_large' => array(
      'width' => 560,
      'height' => 340,
      'autoplay' => FALSE,
    ),
    'media_original' => array(
      'width' => 640,
      'height' => 390,
      'autoplay' => NULL,
    ),
  );
  foreach ($default_video_settings as $view_mode => $settings) {
    $display_name = 'video__' . $view_mode . '__media_ooyala_video';
    $default_displays[$display_name] = (object) array(
      'api_version' => 1,
      'name' => $display_name,
      'status' => 1,
      'weight' => 1,
      'settings' => $settings,
    );
  }
  return $default_displays;
}
