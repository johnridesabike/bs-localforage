type t('value);

module type Data = {
  type t;
  let decode: Js.Json.t => t;
  let encode: t => Js.Json.t;
};

let make: (Config.t, (module Data with type t = 'value)) => t('value);

let getItem: (t('value), ~key: string) => Js.Promise.t(option('value));

let setItem: (t('value), ~key: string, ~v: 'value) => Js.Promise.t(unit);

let getKeys: t('value) => Js.Promise.t(array(string));

let getItems:
  (t('value), ~keys: array(string)) =>
  Js.Promise.t(array((string, 'value)));

let getAllItems: t('value) => Js.Promise.t(array((string, 'value)));

let setItems:
  (t('value), ~items: array((string, 'value))) => Js.Promise.t(unit);

let removeItems: (t('value), ~items: array(string)) => Js.Promise.t(unit);

let iterateU:
  (t('value), ~f: (. 'value, string, int) => unit) => Js.Promise.t(unit);

let iterate:
  (t('value), ~f: ('value, string, int) => unit) => Js.Promise.t(unit);
