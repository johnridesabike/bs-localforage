type t('a);

module type Data = {
  type t;
  let decode: Js.Json.t => t;
  let encode: t => Js.Json.t;
};

let make: (Config.t, (module Data with type t = 'a)) => t('a);

let getItem: (t('a), ~key: string) => Js.Promise.t(option('a));

let setItem: (t('a), ~key: string, ~v: 'a) => Js.Promise.t(unit);

let getKeys: t('a) => Js.Promise.t(array(string));

let getItems:
  (t('a), ~keys: array(string)) => Js.Promise.t(Belt.Map.String.t('a));

let getAllItems: t('a) => Js.Promise.t(Belt.Map.String.t('a));

let setItems: (t('a), ~items: Belt.Map.String.t('a)) => Js.Promise.t(unit);

let removeItems: (t('a), ~items: array(string)) => Js.Promise.t(unit);

let iterateU: (t('a), ~f: (. 'a, string, int) => unit) => Js.Promise.t(unit);

let iterate: (t('a), ~f: ('a, string, int) => unit) => Js.Promise.t(unit);
