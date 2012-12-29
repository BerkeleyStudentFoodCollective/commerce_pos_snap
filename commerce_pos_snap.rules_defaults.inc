<?php

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_pos_snap_default_rules_configuration() {
  $rules = array();
  $variables = array(
  	'commerce_line_item' => array(
  		'type' => 'commerce_line_item',
  		'label' => t('Line Item')
    ),
    'snap_eligibility' => array(
      'type' => 'boolean',
      'label' => t('SNAP Eligibility'),
      'parameter' => FALSE,
    ),
  );
  $provides = array('snap_eligibility');
  $rule_set = rules_rule_set($variables, $provides);
  $rule_set->label = t('Determine SNAP eligibility');

  $rules['commerce_pos_snap_eligibility'] = $rule_set;

  $rule = rules_reaction_rule();
  $rule->event('commerce_payment_transaction_insert')
    ->event('commerce_payment_transaction_update')
    ->condition('data_is', array(
        'data:select' => 'commerce-payment-transaction:payment-method',
        'op' => '==',
        'value' => 'commerce_pos_snap',
      ))
    ->action('commerce_pos_snap_set_tax_exemptions', array(
      'commerce_order:select' => 'commerce-payment-transaction:order',
    ));
    $rule->label = t('Set SNAP tax exemptions when payments are updated.');

    $rules['commerce_pos_snap_set_exemptions'] = $rule;

    $rule = rules_reaction_rule();
    $rule->event('commerce_pos_modify_order_validate')
      ->condition(rules_condition('commerce_pos_snap_order_has_snap_payments', array(
        'commerce_order:select' => 'commerce-order',
      )))
      ->action('commerce_pos_order_set_error', array(
        'commerce_order:select' => 'commerce-order',
        'message' => t('Please remove SNAP payments in order to make changes.')
      ));

    $rules['commerce_pos_snap_no_modify'] = $rule;

    return $rules;
}

/**
 * Implements hook_default_rules_configuration_alter().
 */
function commerce_pos_snap_default_rules_configuration_alter(&$rules) {
  if (!empty($rules['commerce_pos_payment_validate_add'])) {
    $rule_set = $rules['commerce_pos_payment_validate_add'];
    $rule = rule();
    $rule->label = t('Disallow adding SNAP payments above SNAP-eligible amount.');
    $rule->condition(rules_condition('commerce_pos_snap_payment_check_amount', array(
          'commerce_payment_transaction:select' => 'commerce-payment-transaction',
          'commerce_order:select' => 'commerce-order',
          'value' => 'pos_in_progress',
          ))->negate())
      ->action('list_add', array(
          'list:select' => 'errors',
          'item' => t('You cannot add a SNAP payment of that size.'),
        ));
    $rule_set->rule($rule);
  }
}