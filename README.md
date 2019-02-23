EFQ Rules exposes Drupal EFQ API to rules, so that entities can be queried and fetched based on multiple fields, properties and conditions.

There is a tutorial on how to use EFQ here, And this is the API documentation.

Almost all methods from EFQ's interface are available, and a new data type for creating new instances of EFQ itself is introduced. There are still limitations which hopefully we'll get over soon, such as you can not still pass arrays of values.

A similar approach is Views Rules where entities from a view can be loaded and looped (and the view is configurable, good!).

Please note: Do not use 'add a variable of type EFQ' in rules, and use the action 'New EntityFieldQuery' instead.
