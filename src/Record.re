module LF = LocalForageJs;
include LoadAllPlugins;
type t('a) = {
  store: LocalForageJs.t,
  default: 'a,
  encode: 'a => Js.Json.t,
  decode: Js.Json.t => 'a,
};
type config = LocalForageJs.Config.t;
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
  |> Js.Promise.then_(items =>
       switch (decode(items)) {
       | exception error =>
         Js.Console.error(error);
         Js.Console.warn("Failed to decode database. Using default record.");
         Js.Promise.resolve(default);
       | result => Js.Promise.resolve(result)
       }
     );
let set = ({store, encode}, ~items) => {
  SetItemsJs.fromJson(store, encode(items));
};