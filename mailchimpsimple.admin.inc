<?php

use \DrewM\MailChimp\MailChimp;

/**
 * Helloasso configuration form.
 */
function mailchimpsimple_admin_form($form, &$form_state) {
  $form = array();
  $form['#tree'] = FALSE;

  // Get config
  $config = config('custom.mailchimpsimple');
  $form['#config'] = 'custom.mailchimpsimple';

  // API Key field.
  $mailchimpsimple_api_key = mailchimpsimple_get_api_key();
  $form['mailchimpsimple_api_key'] = array(
    '#title' => t('API Key'),
    '#type' => 'textfield',
    '#default_value' => $mailchimpsimple_api_key,
    '#required' => TRUE,
  );

  if (!empty($mailchimpsimple_api_key)) {
    // Use MailChimp API
    $mailchimpsimple = new MailChimp($mailchimpsimple_api_key);

    // Get available lists and default list
    $lists = $mailchimpsimple->get('lists');

    // Free MailChimp account can only have one list.
    if ($lists['total_items'] == 1) {
      // Automatically define the default_list config.
      $config->set('mailchimpsimple_default_list', $lists['lists'][0]['id']);
    }

    // Get default list
    $default_list = mailchimpsimple_get_default_list();
    if (!$default_list) {
      backdrop_set_message(t("You must define a default list in order to have a working subscription block"), 'warning');
    }

    $lists_options = array();
    if (!empty($lists)) {
      foreach ($lists['lists'] as $list) {
        $lists_options[$list['id']] = $list['name'];
      }
    }
    $form['mailchimpsimple_default_list'] = array(
      '#title' => t("Default list"),
      '#type' => 'select',
      '#options' => $lists_options,
      '#default_value' => $default_list,
      '#required' => TRUE,
    );
  }

  return system_settings_form($form);
}
