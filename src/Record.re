module LF = LocalForageJs;
module P = Js.Promise;
include LoadAllPlugins;

type t('a, 'identity) = {
  store: LocalForageJs.t,
  encode: (. 'a) => Js.Json.t,
  decode: (. Js.Json.t) => 'a,
};

let make = (config, type t, type id, data: Id.encodable(t, id)) => {
  module Data = (val data);
  let encode = Id.encode(Data.encode);
  let decode = Id.decode(Data.decode);
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
