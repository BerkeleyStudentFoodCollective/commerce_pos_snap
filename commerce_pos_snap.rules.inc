<?php

/**
 * Implements hook_rules_action_info().
 */
function commerce_pos_snap_rules_action_info() {
  $actions = array();

  $actions['commerce_pos_snap_set_tax_exemptions'] = array(
    'label' => t('Set tax exemptions'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
    ),
    'group' => t('Commerce POS Order'),
    'callbacks' => array(
      'execute' => 'commerce_pos_snap_rules_set_tax_exemptions',
    ),
  );
  return $actions;
}

/**
 * Implements hook_rules_condition_info().
 */
function commerce_pos_snap_rules_condition_info() {
  $conditions = array();

  $conditions['commerce_pos_snap_payment_check_amount'] = array(
    'label' => t('Check SNAP payment amount'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
      'commerce_payment_transaction' => array(
        'type' => 'commerce_payment_transaction',
        'label' => t('Payment transaction'),
      ),
    ),
    'group' => t('Commerce POS Payment'),
    'callbacks' => array(
      'execute' => 'commerce_pos_snap_check_payment_amount',
    ),
  );
  $conditions['commerce_pos_snap_order_has_snap_payments'] = array(
    'label' => t('Order has SNAP payments'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
    ),
    'group' => t('Commerce POS Payment'),
    'callbacks' => array(
      'execute' => 'commerce_pos_snap_order_has_snap_payments',
    ),
  );
  return $conditions;
}

/**
 * Execute callback for setting tax exemptions via rules action.
 */
function commerce_pos_snap_rules_set_tax_exemptions($order) {
  commerce_pos_snap_apply_tax_exemptions($order);
}

/**
 * Execute callback for for payment amount check.
 */
function commerce_pos_snap_check_payment_amount($order, $payment) {
  if ($payment->payment_method != 'commerce_pos_snap') {
    return TRUE;
  }
  $snap_paid = commerce_pos_snap_snap_payment_total($order);
  $eligible = commerce_pos_snap_calculate_order_snap_eligible($order);

  return commerce_currency_convert($snap_paid['amount'], $snap_paid['currency_code'], $eligible['currency_code'])
    + commerce_currency_convert($payment->amount, $payment->currency_code, $eligible['currency_code'])
    <= $eligible['amount'];
}

/**
 * Execute callback for checking for existence of SNAP payments.
 */
function commerce_pos_snap_order_has_snap_payments($order) {
  $transaction_statuses = commerce_payment_transaction_statuses();

  $transactions = commerce_payment_transaction_load_multiple(array(), array('order_id' => $order->order_id, 'payment_method' => 'commerce_pos_snap'));
  foreach ($transactions as $transaction) {
    if ($transaction_statuses[$transaction->status]['total']) {
      return TRUE;
    }
  }
  return FALSE;
}