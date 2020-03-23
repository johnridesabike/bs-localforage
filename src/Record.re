module LF = LocalForageJs;
module P = Js.Promise;
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
  |> P.then_(items =>
       switch (decode(. items)) {
       | exception error => P.reject(error)
       | items => P.resolve(items)
       }
     );

let set = ({store, encode, _}, ~items) => {
  SetItemsJs.fromJson(store, encode(. items));
};
