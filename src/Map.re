module D = Js.Dict;
module LF = LocalForageJs;
module P = Js.Promise;
module A = Js.Array;
include LoadAllPlugins;

type t('a) = {
  store: LocalForageJs.t,
  decode: (. Js.Json.t) => 'a,
  encode: (. 'a) => Js.Json.t,
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

let getItem = ({store, decode, _}, ~key) => {
  LF.getItem(store, key)
  |> P.then_(value =>
       switch (value->Js.Nullable.toOption->Belt.Option.mapU(decode)) {
       | exception error => P.reject(error)
       | value => P.resolve(value)
       }
     );
};

let setItem = ({store, encode, _}, ~key, ~v) =>
  LF.setItem(store, key, encode(. v));

let getKeys = ({store, _}) => LF.keys(store);

let mapValues = ((key, value), ~f) => (key, f(. value));

let parseItems = (decode, items) =>
  items |> D.entries |> A.map(mapValues(~f=decode));

let getItems = ({store, decode, _}, ~keys) =>
  GetItemsJs.dictFromArray(store, keys)
  |> P.then_(items =>
       switch (parseItems(decode, items)) {
       | exception error => P.reject(error)
       | items => P.resolve(items)
       }
     );

let getAllItems = ({store, decode, _}) =>
  GetItemsJs.allDict(store)
  |> P.then_(items =>
       switch (parseItems(decode, items)) {
       | exception error => P.reject(error)
       | items => P.resolve(items)
       }
     );

let setItems = ({store, encode, _}, ~items) => {
  items
  |> A.map(mapValues(~f=encode))
  |> D.fromArray
  |> SetItemsJs.fromDict(store);
};

let removeItems = ({store, _}, ~items) =>
  RemoveItemsJs.fromArray(store, items);

let iterateU = ({store, decode, _}, ~f) => {
  LF.iterate(store, (value, key, iterationNumber) =>
    f(. decode(. value), key, iterationNumber)
  );
};

let iterate = (map, ~f) =>
  iterateU(map, ~f=(. value, key, num) => f(value, key, num));
