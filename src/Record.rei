type t('a);
type config = LocalForageJs.config;
module type Data = {
  type t;
  let default: t;
  let decode: Js.Json.t => t;
  let encode: t => Js.Json.t;
};
let make: (config, (module Data with type t = 'a)) => t('a);
let get: t('a) => Js.Promise.t('a);
let set: (t('a), ~items: 'a) => Js.Promise.t(unit);