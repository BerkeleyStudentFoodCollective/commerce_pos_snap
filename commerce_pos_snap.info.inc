<?php

/**
 * @file
 * Adds the food stamp amount to the order and line item entities.
 */

/**
 * Implements hook_entity_property_info_alter().
 */
function commerce_pos_snap_entity_property_info_alter(&$info) {
  $info['commerce_order']['properties']['commerce_pos_snap_amount'] = array(
    'label' => t('SNAP Amount'),
    'description' => t('The SNAP-eligible amount for this order.'),
    'type' => 'decimal',
    'getter callback' => '_commerce_pos_snap_order_snap_amount',
  );
  $info['commerce_order']['properties']['commerce_pos_snap_currency_code'] = array(
    'label' => t('SNAP Currency Code'),
    'description' => t('The currency code of the SNAP amount for this order.'),
    'type' => 'text',
    'getter callback' => '_commerce_pos_snap_order_snap_currency_code',
  );
  $info['commerce_line_item']['properties']['commerce_pos_snap_amount'] = array(
    'label' => t('SNAP Amount'),
    'description' => t('The SNAP-eligible amount for this line item.'),
    'type' => 'decimal',
    'getter callback' => '_commerce_pos_snap_line_item_snap_amount',
  );
  $info['commerce_line_item']['properties']['commerce_pos_snap_currency_code'] = array(
    'label' => t('SNAP Currency Code'),
    'description' => t('The currency code of the SNAP amount for this line item.'),
    'type' => 'text',
    'getter callback' => '_commerce_pos_snap_line_item_snap_currency_code',
  );
}