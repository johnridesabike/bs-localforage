let errorHandler: 'a => Js.String.t;
type t('a);
type config = LocalForageJs.config;
module type Data = {
  type t;
  let default: t;
  let decode: Js.Json.t => t;
  let encode: t => Js.Json.t;
};
let make: (config, (module Data with type t = 'a)) => t('a);
let get: t('a) => Future.t(Belt.Result.t('a, Js.String.t));
let set: (t('a), ~items: 'a) => Future.t(Belt.Result.t(unit, Js.String.t));