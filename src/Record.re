module LF = LocalForageJs;
include LoadAllPlugins;

type t('a) = {
  store: LocalForageJs.t,
  encode: (. 'a) => Js.Json.t,
  decode: (. Js.Json.t) => 'a,
};

module type Data = {
  type t;
  let decode: Js.Json.t => t;
  let encode: t => Js.Json.t;
};

let make = (config, type t, data: (module Data with type t = t)) => {
  module Data = (val data);
  let encode = {
    let encode = Data.encode;
    (. x) => encode(x);
  };
  let decode = {
    let decode = Data.decode;
    (. x) => decode(x);
  };
  {store: LF.make(config), encode, decode};
};

let get = ({store, decode, _}) =>
  GetItemsJs.allJson(store)
  |> Js.Promise.then_(items => decode(. items) |> Js.Promise.resolve);

let set = ({store, encode, _}, ~items) => {
  SetItemsJs.fromJson(store, encode(. items));
};
