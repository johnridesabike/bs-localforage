open Belt.Result;
module LF = LocalForageJs;
include LoadAllPlugins;
let errorHandler = error => {
  Js.Console.error(error);
  Js.String.make(error);
};
type t('a) = {
  store: LocalForageJs.t,
  default: 'a,
  encode: 'a => Js.Json.t,
  decode: Js.Json.t => 'a,
};
type config = LocalForageJs.config;
module type Data = {
  type t;
  let default: t;
  let decode: Js.Json.t => t;
  let encode: t => Js.Json.t;
};
let make = (config, type t, data: (module Data with type t = t)) => {
  module Data = (val data);
  {
    store: LF.make(config),
    default: Data.default,
    encode: Data.encode,
    decode: Data.decode,
  };
};
let get = ({store, decode, default}) =>
  GetItemsJs.allJson(store)
  ->FutureJs.fromPromise(errorHandler)
  ->Future.flatMapOk(items =>
      switch (decode(items)) {
      | exception x =>
        Js.Console.error(x);
        Future.value(Ok(default));
      | x => Future.value(Ok(x))
      }
    );
let set = ({store, encode}, ~items) => {
  SetItemsJs.fromJson(store, encode(items))
  ->FutureJs.fromPromise(errorHandler);
};