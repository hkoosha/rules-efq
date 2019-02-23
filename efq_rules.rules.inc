<?php

/**
 * @file
 * Provides rule integration for EntityFieldQuery.
 */

/**
 * Implements hook_rules_data_info().
 *
 * Add wrapper so conditions can be 1. accessible/debugable 2.set-able via set 
 * a data value.
 */
function efq_rules_rules_data_info() {
  return array(
    'efq' => array(
      'label' => t('EntityFieldQuery'),
    ),
  );
}

/**
 * Implements hook_rules_action_info().
 *
 * @TODO EntityFieldQuery::addCondition, EntityFieldQuery::initializePager,
 * EntityFieldQuery::pager, EntityFieldQuery::tableSort.
 */
function efq_rules_rules_action_info() {
  $action['efq_rules_action_new_efq'] = array(
    'label' => t('New EntitiFieldQuery'),
    'group' => t('EFQ'),
    'provides' => array(
      'efq' => array(
        'type' => 'efq',
        'label' => 'EntityFieldQuery',
      ),
    ),
  );

  $action['efq_rules_action_fetch_entities_ekran'] = array(
    'label' => t('Execute EFQ and fetch entities'),
    'group' => t('EFQ'),
    'parameter' => array(
      'efq' => array(
        'type' => 'efq',
        'label' => t('EFQ'),
      ),
      'entity_type' => array(
        'type' => 'token',
        'label' => t('Entity type'),
        'options list' => '_efq_rules_value_callback',
      ),
    ),
    'provides' => array(
      'fetched_entities' => array(
        'type' => 'list<entity>',
        'label' => t('Fetched entities'),
      ),
    ),
  );

  // @TODO Declare 1 actions per entity type and return those entities.
  // No need to care for the esoteric case of cross-entity EFQs now.
  $action['efq_rules_action_ekran'] = array(
    'label' => t('Execute EFQ and get IDs (DEPRECATED)'),
    'group' => t('EFQ'),
    'parameter' => array(
      'efq' => array(
        'type' => 'efq',
        'label' => t('EFQ'),
      ),
      'entity_type' => array(
        'type' => 'token',
        'label' => t('Entity type'),
        'options list' => '_efq_rules_value_callback',
      ),
    ),
    'provides' => array(
      'result_ids' => array(
        'type' => 'list<integer>',
        'label' => t('Result IDs'),
      ),
    ),
  );

  $action['efq_rules_action_field_condition'] = array(
    'label' => t('Adds a field value condition'),
    'group' => t('EFQ'),
    'parameter' => array(
      'efq' => array(
        'type' => 'efq',
        'label' => t('EFQ'),
      ),
      'field' => array(
        // Contrary to EFQ docs only single value allowed, array throws on execute.
        'type' => 'token',
        'label' => t('Fields'),
        'description' => t('The field the condition applies.'),
        'options list' => '_efq_rules_value_callback',
      ),
      'column' => array(
        'type' => 'text',
        'label' => t('Column'),
        'optional' => TRUE,
        'allow null' => TRUE,
        'description' => t('The column that should hold the value to be matched.'),
      ),
      'value' => array(
        'type' => 'text',
        'label' => t('Value'),
        'optional' => TRUE,
        'allow null' => TRUE,
        'description' => t('The value to test the column value against.'),
      ),
      'operator' => array(
        'type' => 'token',
        'label' => t('Operator'),
        'options list' => '_efq_rules_value_callback',
      ),
      'delta_group' => array(
        'type' => 'text',
        'label' => t('Delta group'),
        'optional' => TRUE,
        'allow null' => TRUE,
        'description' => t('An arbitrary identifier: conditions in the same group must have the same $delta_group.'),
      ),
      'language_group' => array(
        'type' => 'text',
        'label' => t('Language group'),
        'optional' => TRUE,
        'allow null' => TRUE,
        'description' => t('An arbitrary identifier: conditions in the same group must have the same $language_group.'),
      ),
    ),
  );

  $action['efq_rules_action_add_tag'] = array(
    'label' => t('Add a tag to the query.'),
    'group' => t('EFQ'),
    'parameter' => array(
      'efq' => array(
        'type' => 'efq',
        'label' => t('EFQ'),
      ),
      'tag' => array(
        'type' => 'text',
        'label' => t('Tag'),
      ),
    ),
  );

  $action['efq_rules_action_set_flags'] = array(
    'label' => t('Set available flags on EFQ'),
    'group' => t('EFQ'),
    'parameter' => array(
      'efq' => array(
        'type' => 'efq',
        'label' => t('EFQ'),
      ),
      'deleted' => array(
        'type' => 'token',
        'label' => t('Deleted'),
        'description' => t('Filters on the data being deleted.'),
        'options list' => '_efq_rules_value_callback',
        'optional' => TRUE,
        'allow null' => TRUE,
      ),
      'count' => array(
        'type' => 'token',
        'label' => t('Count'),
        'description' => t('Sets the query to be a count query only.'),
        'options list' => '_efq_rules_value_callback',
        'optional' => TRUE,
        'allow null' => TRUE,
      ),
      'age' => array(
        'type' => 'token',
        'label' => t('Age'),
        'options list' => '_efq_rules_value_callback',
        'optional' => TRUE,
        'allow null' => TRUE,
      ),
    ),
  );

  // @TODO Value can be array (And possible values of operator changes too).
  $action['efq_rules_action_property_condition'] = array(
    'label' => t('Add a condition on an entity-specific property to EFQ'),
    'group' => t('EFQ'),
    'parameter' => array(
      'efq' => array(
        'type' => 'efq',
        'label' => t('EFQ'),
      ),
      'column' => array(
        'type' => 'text',
        'label' => t('Column'),
        'description' => t('A column defined in the hook_schema() of the base table of the entity.'),
      ),
      'value' => array(
        'type' => 'text',
        'label' => t('Value'),
        'description' => t('The value to test the field against.'),
      ),
      'operator' => array(
        'type' => 'token',
        'label' => t('Operator'),
        'options list' => '_efq_rules_value_callback',
      ),
    ),
  );

  $action['efq_rules_action_field_order_by'] = array(
    'label' => t('Order EFQ result by field column'),
    'group' => t('EFQ'),
    'parameter' => array(
      'efq' => array(
        'type' => 'efq',
        'label' => t('EFQ'),
      ),
      'field' => array(
        'type' => 'token',
        'label' => t('Field'),
        'options list' => '_efq_rules_value_callback',
        'description' => t('The field name.'),
      ),
      'column' => array(
        'type' => 'text',
        'label' => t('Column'),
        'description' => t('A column defined in the hook_field_schema() of selected field. entity_id and bundle can also be used.'),
      ),
      'direction' => array(
        'type' => 'token',
        'label' => t('Direction'),
        'options list' => '_efq_rules_value_callback',
        'description' => t('The direction to sort.'),
        'default value' => 'ASC',
      ),
    ),
  );

  $action['efq_rules_action_deleted'] = array(
    'label' => t('Filter result on data being deleted'),
    'group' => t('EFQ'),
    'parameter' => array(
      'efq' => array(
        'type' => 'efq',
        'label' => t('EFQ'),
      ),
      'deleted' => array(
        'type' => 'token',
        'label' => t('Deleted'),
        'options list' => '_efq_rules_value_callback',
        'description' => t('TRUE to only return deleted data, FALSE to return non-deleted data.'),
        'default value' => EntityFieldQuery::RETURN_ALL,
      ),
    ),
  );

  $action['efq_rules_action_count'] = array(
    'label' => t('Sets the query to be a count query only.'),
    'group' => t('EFQ'),
    'parameter' => array(
      'efq' => array(
        'type' => 'efq',
        'label' => t('EFQ'),
        'description' => t('Sets the query to be a count query only.'),
      ),
    ),
  );

  $action['efq_rules_action_property_order_by'] = array(
    'label' => t('Order EFQ result by entity-specific property'),
    'group' => t('EFQ'),
    'parameter' => array(
      'efq' => array(
        'type' => 'efq',
        'label' => t('EFQ'),
      ),
      'column' => array(
        'type' => 'text',
        'label' => t('Column'),
        'description' => t('The column on which to order.'),
      ),
      'direction' => array(
        'type' => 'token',
        'label' => t('Direction'),
        'options list' => '_efq_rules_value_callback',
        'description' => t('The direction to sort.'),
        'default value' => 'ASC',
      ),
    ),
  );

  $action['efq_rules_action_range'] = array(
    'label' => t('Add range to EFQ'),
    'group' => t('EFQ'),
    'parameter' => array(
      'efq' => array(
        'type' => 'efq',
        'label' => t('EFQ'),
      ),
      'start' => array(
        'type' => 'integer',
        'label' => t('Start'),
        'description' => t('The first entity from the result set to return. If no value is given, removes any range directives that are set.'),
        'optional' => TRUE,
        'allow null' => TRUE,
      ),
      'length' => array(
        'type' => 'integer',
        'label' => t('Length'),
        'optional' => TRUE,
        'description' => t('The number of entities to return from the result set.'),
        'allow null' => TRUE,
      ),
    ),
  );

  $action['efq_rules_action_age'] = array(
    'label' => t('Queries the current/every revision'),
    'group' => t('EFQ'),
    'parameter' => array(
      'efq' => array(
        'type' => 'efq',
        'label' => t('EFQ'),
      ),
      'age' => array(
        'type' => 'token',
        'label' => t('Age'),
        'options list' => '_efq_rules_value_callback',
      ),
    ),
  );

  $action['efq_rules_action_add_metadata'] = array(
    'label' => t('Add metadata to the query'),
    'group' => t('EFQ'),
    'parameter' => array(
      'efq' => array(
        'type' => 'efq',
        'label' => t('EFQ'),
      ),
      // @TODO validate?
      'key' => array(
        'type' => 'text',
        'label' => t('Key'),
        'description' => t('The unique identifier for this piece of metadata. Must be a string that follows the same rules as any other PHP identifier.'),
      ),
      'object' => array(
        'type' => '*',
        'label' => t('Object'),
        'description' => t('The additional data to add to the query. May be any valid PHP variable.'),
      ),
    ),
  );

  $action['efq_rules_action_entity_order_by'] = array(
    'label' => t('Order result set by entity-generic metadata.'),
    'parameter' => array(
      'efq_object' => array(
        'type' => 'efq',
        'label' => t('EFQ'),
      ),
      'name' => array(
        'type' => 'token',
        'label' => t('Operator'),
        'options list' => '_efq_rules_value_callback',
      ),
      'direction' => array(
        'type' => 'token',
        'label' => t('Entity condition'),
        'options list' => '_efq_rules_value_callback',
        'optional' => TRUE,
        'default value' => 'ASC',
      ),
    ),
    'group' => t('EFQ'),
  );

  $action['efq_rules_action_add_entity_condition'] = array(
    'label' => t('Add entity condition to EFQ'),
    'group' => t('EFQ'),
    'parameter' => array(
      'efq_object' => array(
        'type' => 'efq',
        'label' => t('EFQ'),
      ),
      'operator' => array(
        'type' => 'token',
        'label' => t('Operator'),
        'options list' => '_efq_rules_value_callback',
        'allow null' => TRUE,
        'optional' => TRUE,
      ),
      'entity_condition' => array(
        'type' => 'token',
        'label' => t('Entity condition'),
        'options list' => '_efq_rules_value_callback',
        'optional' => TRUE,
      ),
      'entity_type' => array(
        'type' => 'token',
        'label' => t('Entity Type'),
        'options list' => '_efq_rules_value_callback',
        'optional' => TRUE,
      ),
      'bundle' => array(
        'type' => 'token',
        'label' => t('Bundle'),
        'options list' => '_efq_rules_value_callback',
        'optional' => TRUE,
      ),
      'entity_id' => array(
        'type' => 'integer',
        'label' => t('Entity ID'),
        'optional' => TRUE,
      ),
      'revision_id' => array(
        'type' => 'integer',
        'label' => t('Revision ID'),
        'optional' => TRUE,
      ),
    ),
    'callbacks' => array(
      'form_alter' => '_efq_rules_config_form_alter',
    ),
  );

  $action['efq_rules_action_field_language_condition'] = array(
    'label' => t('Add condition on field language column to EFQ'),
    'group' => t('EFQ'),
    'parameter' => array(
      'efq_object' => array(
        'type' => 'efq',
        'label' => t('EFQ'),
      ),
      'field' => array(
        'type' => 'token',
        'label' => t('Field'),
        'options list' => '_efq_rules_value_callback',
        'description' => t('Either a field name or a field array'),
      ),
      'value' => array(
        'type' => 'text',
        'label' => t('Value'),
        'optional' => TRUE,
        'description' => t('The value to test the column value against.'),
        'allow null' => TRUE,
      ),
      'operator' => array(
        'type' => 'token',
        'label' => t('Operator'),
        'options list' => '_efq_rules_value_callback',
        'optional' => TRUE,
        'allow null' => TRUE,
      ),
      'delta_group' => array(
        'type' => 'text',
        'label' => t('Delta group'),
        'optional' => TRUE,
        'allow null' => TRUE,
        'description' => t('An arbitrary identifier: conditions in the same group must have the same $delta_group.'),
      ),
      'language_group' => array(
        'type' => array('text'),
        'label' => t('Language group'),
        'optional' => TRUE,
        'allow null' => TRUE,
        'description' => t('An arbitrary identifier: conditions in the same group must have the same $language_group.'),
      ),
    ),
  );

  $action['efq_rules_action_field_delta_condition'] = array(
    'label' => t('Add condition on field delta column to EFQ'),
    'group' => t('EFQ'),
    'parameter' => array(
      'efq_object' => array(
        'type' => 'efq',
        'label' => t('EFQ'),
      ),
      'field' => array(
        'type' => 'token',
        'label' => t('Field'),
        'options list' => '_efq_rules_value_callback',
        'description' => t('Either a field name or a field array.'),
      ),
      'value' => array(
        'type' => 'text',
        'label' => t('Value'),
        'optional' => TRUE,
        'description' => t('The value to test the column value against.'),
        'allow null' => TRUE,
      ),
      'operator' => array(
        'type' => 'token',
        'label' => t('Operator'),
        'options list' => '_efq_rules_value_callback',
        'optional' => TRUE,
        'allow null' => TRUE,
      ),
      'delta_group' => array(
        'type' => 'text',
        'label' => t('Delta group'),
        'optional' => TRUE,
        'allow null' => TRUE,
        'description' => t('An arbitrary identifier: conditions in the same group must have the same $delta_group.'),
      ),
      'language_group' => array(
        'type' => array('text'),
        'label' => t('Language group'),
        'optional' => TRUE,
        'allow null' => TRUE,
        'description' => t('An arbitrary identifier: conditions in the same group must have the same $language_group.'),
      ),
    ),
  );

  return $action;
}

/**
 * Generated possible values of action arguments.
 */
function _efq_rules_value_callback($action, $parameter) {
  switch ($parameter) {
    case 'age':
      $values = array(
        FIELD_LOAD_CURRENT => t('FIELD_LOAD_CURRENT'),
        FIELD_LOAD_REVISION => t('FIELD_LOAD_REVISION'),
      );
      break;

    case 'field':
      $values = array();
      foreach (field_info_fields() as $name => $f) {
        $values[$name] = check_plain($name);
      }
      break;

    case 'deleted':
    case 'truefalse':
      $values = array(
        1 => t('True'),
        0 => t('False'),
      );
      break;

    case 'operator':
      $values = array(
        '=' => '=',
        '<>' => '<>',
        '>' => '>',
        '>=' => '>=',
        '<' => '<',
        '<=' => '<=',
        'STARTS_WITH' => 'STARTS_WITH',
        'CONTAINS' => 'CONTAINS',
        'IN' => 'IN',
        'NOT IN' => 'NOT IN',
        'BETWEEN' => 'BETWEEN',
      );
      break;

    case 'bundle':
      $values = array();
      foreach (entity_get_info() as $e_name => $e) {
        if (isset($e['bundles'])) {
          foreach ($e['bundles'] as $name => $b) {
            if (isset($values[$name])) {
              // For two entity types but bundles with same name.
              $values[$name] = $values[$name] . check_plain(" - " . $e_name);
            }
            else {
              $values[$name] = check_plain($b['label'] . ": " . $e_name);
            }
          }
        }
      }
      break;

    case 'entity_type':
      $values = array();
      foreach (entity_get_info() as $name => $e) {
        $values[$name] = check_plain(isset($e['label']) ? $e['label'] : $name);
      }
      break;

    case 'name':
    case 'entity_condition':
      $values = array(
        'entity_type' => t('Entity type'),
        'bundle' => t('Bundle'),
        'revision_id' => t('Revision ID'),
        'entity_id' => t('Entity ID'),
      );
      break;

    case 'direction':
      $values = array(
        'ASC' => t('ASC'),
        'DESC' => t('DESC'),
      );
      break;
  }

  drupal_alter('efq_rules_value_callback', $values, $parameter);
  return $values;
}

/**
 * Returns a new EntityieldQuery to Rules.
 */
function efq_rules_action_new_efq() {
  return array('efq' => new EntityFieldQuery());
}

/**
 * Look at EntityFieldQuery for more info.
 */
function efq_rules_action_add_entity_condition(EntityFieldQuery $efq, $operator, $entity_condition, $entity_type, $bundle, $entity_id, $revision_id) {
  $conditions = compact('entity_type', 'bundle', 'entity_id', 'revision_id');
  $efq->entityCondition($entity_condition, $conditions[$entity_condition], $operator);
}

/**
 * Look at EntityFieldQuery for more info.
 */
function _efq_rules_config_form_alter(&$form, &$form_state, $options, RulesAbstractPlugin $element) {
  $first_step = empty($element->settings['entity_condition']);
  $form['reload'] = array(
    '#weight' => 5,
    '#type' => 'submit',
    '#name' => 'reload',
    '#value' => $first_step ? t('Continue') : t('Reload form'),
    '#limit_validation_errors' => array(array('efq_object', 'condition_name')),
    '#submit' => array('rules_action_type_form_submit_rebuild'),
    '#ajax' => rules_ui_form_default_ajax(),
  );
  // Use ajax and trigger as the reload button.
  $form['parameter']['entity_condition']['settings']['entity_condition']['#ajax'] = $form['reload']['#ajax'] + array(
    'event' => 'change',
    'trigger_as' => array('name' => 'reload'),
  );

  $keep = array('efq_object', 'entity_condition', 'operator');
  if ($first_step) {
    // In the first step show only the type select.
    foreach (element_children($form['parameter']) as $key) {
      if (!(in_array($key, $keep))) {
        unset($form['parameter'][$key]);
      }
    }
    unset($form['submit']);
    unset($form['provides']);
    // Disable #ajax for the first step as it has troubles with lazy-loaded JS.
    // @todo: Re-enable once JS lazy-loading is fixed in core.
    unset($form['parameter']['type']['settings']['type']['#ajax']);
    unset($form['reload']['#ajax']);
  }
  else {
    // Hide the reload button in case js is enabled and it's not the first step.
    $form['reload']['#attributes'] = array('class' => array('rules-hide-js'));
    $keep[] = $element->settings['entity_condition'];

    foreach (element_children($form['parameter']) as $key) {
      if (!(in_array($key, $keep))) {
        unset($form['parameter'][$key]);
      }
    }
  }
}

/**
 * Look at EntityFieldQuery for more info.
 */
function efq_rules_action_range(EntityFieldQuery $efq, $start, $length) {
  $efq->range($start, $length);
}

/**
 * Look at EntityFieldQuery for more info.
 */
function efq_rules_action_add_tag(EntityFieldQuery $efq, $tag) {
  $efq->addTag($tag);
}

/**
 * Look at EntityFieldQuery for more info.
 */
function efq_rules_action_property_condition(EntityFieldQuery $efq, $column, $value, $operator) {
  $efq->propertyCondition($column, $value, $operator);
}

/**
 * Look at EntityFieldQuery for more info.
 */
function efq_rules_action_property_order_by(EntityFieldQuery $efq, $column, $direction) {
  $efq->propertyCondition($column, $direction);
}

/**
 * Look at EntityFieldQuery for more info.
 */
function efq_rules_action_field_order_by(EntityFieldQuery $efq, $field, $column, $direction) {
  $efq->fieldOrderBy($field, $column, $direction);
}

/**
 * Look at EntityFieldQuery for more info.
 */
function efq_rules_action_field_condition(EntityFieldQuery $efq, $field, $column, $value, $operator, $delta_group, $language_group) {
  $efq->fieldCondition($field, $column, $value, $operator, $delta_group, $language_group);
}

/**
 * Look at EntityFieldQuery for more info.
 */
function efq_rules_action_set_flags(EntityFieldQuery $efq, $deleted, $count, $age) {
  if ($deleted !== NULL) {
    efq_rules_action_deleted($efq, $deleted);
  }
  if ($count !== NULL) {
    efq_rules_action_count($efq);
  }
  if ($age !== NULL) {
    efq_rules_action_age($efq, $age);
  }
}

/**
 * Returns ids of found entities if any.
 *
 * Look at EntityFieldQuery for more info.
 */
function efq_rules_action_ekran(EntityFieldQuery $efq, $entity_type) {
  $result = $efq->execute();

  $ret = isset($result[$entity_type]) ? array_keys($result[$entity_type]) : array();

  return array('result_ids' => $ret);
}

/**
 * Returns loaded entities based on found ids if any.
 *
 * Look at EntityFieldQuery for more info.
 */
function efq_rules_action_fetch_entities_ekran(EntityFieldQuery $efq, $entity_type) {
  $result = $efq->execute();

  $entity_ids = isset($result[$entity_type]) ? array_keys($result[$entity_type]) : array();
  // EntityListWrapper is broken for everything but IDs - https://drupal.org/node/1013428
  $entities = entity_metadata_wrapper("list<$entity_type>", $entity_ids);
  return array('fetched_entities' => $entities);
}

function efq_rules_action_fetch_entities_ekran_assertions(RulesActionInterface $element) {
  $result_key = 'fetched_entities:var';
  $settings = $element->settings;
  if (isset($settings['entity_type'])) {
    $type = $element->settings['entity_type'];
    $result_variable_name = $settings[$result_key];
    $return = array($result_variable_name => array('type' => "list<$type>"));
    return $return;
  }
}

/**
 * Look at EntityFieldQuery for more info.
 */
function efq_rules_action_field_language_condition(EntityFieldQuery $efq, $field, $value, $operator, $delta_group, $language_group) {
  $efq->fieldLanguageCondition($field, $value, $operator, $delta_group, $language_group);
}

/**
 * Look at EntityFieldQuery for more info.
 */
function efq_rules_action_field_delta_condition(EntityFieldQuery $efq, $field, $value, $operator, $delta_group, $language_group) {
  $efq->fieldDeltaCondition($field, $value, $operator, $delta_group, $language_group);
}

/**
 * Look at EntityFieldQuery for more info.
 */
function efq_rules_action_entity_order_by(EntityFieldQuery $efq, $name, $direction) {
  $efq->entityOrderBy($name, $direction);
}

/**
 * Look at EntityFieldQuery for more info.
 */
function efq_rules_action_count(EntityFieldQuery $efq) {
  $efq->count();
}

/**
 * Look at EntityFieldQuery for more info.
 */
function efq_rules_action_deleted(EntityFieldQuery $efq, $deleted) {
  $efq->deleted($deleted);
}

/**
 * Look at EntityFieldQuery for more info.
 */
function efq_rules_action_age(EntityFieldQuery $efq, $age) {
  $efq->age($age);
}

/**
 * Look at EntityFieldQuery for more info.
 */
function efq_rules_action_add_metadata(EntityFieldQuery $efq, $key, $object) {
  $efq->addMetaData($key, $object);
}
