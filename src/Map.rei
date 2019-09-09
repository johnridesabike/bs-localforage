let errorHandler: 'a => Js.String.t;
type t('a);
type config = LocalForageJs.config;
module type Data = {
  type t;
  let decode: Js.Json.t => t;
  let encode: t => Js.Json.t;
};
let make: (config, (module Data with type t = 'a)) => t('a);
let getItem:
  (t('a), ~key: string) => Future.t(Belt.Result.t(option('a), Js.String.t));
let setItem: (t('a), ~key: string, ~v: 'a) => Js.Promise.t(unit);
let getKeys: t('a) => Future.t(Belt.Result.t(array(string), unit));
let getItems:
  (t('a), ~keys: array(string)) =>
  Future.t(Belt.Result.t(Belt.Map.String.t('a), Js.String.t));
let getAllItems:
  t('a) => Future.t(Belt.Result.t(Belt.Map.String.t('a), Js.String.t));
let setItems:
  (t('a), ~items: Belt.Map.String.t('a)) =>
  Future.t(Belt.Result.t(unit, Js.String.t));
let removeItems:
  (t('a), ~items: array(string)) =>
  Future.t(Belt.Result.t(unit, Js.String.t));
let iterate:
  (t('a), ~f: (. 'a, string, int) => unit) =>
  Future.t(Belt.Result.t(unit, Js.String.t));