module M = Belt.Map.String;
module LF = LocalForageJs;
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
  |> Js.Promise.then_(value =>
       switch (value->Js.Nullable.toOption->Belt.Option.mapU(decode)) {
       | Some(value) => Js.Promise.resolve(Some(value))
       | None => Js.Promise.resolve(None)
       }
     );
};

let setItem = ({store, encode, _}, ~key, ~v) =>
  LF.setItem(store, key, encode(. v));

let getKeys = ({store, _}) => LF.keys(store);

let parseItems = (decode, items) =>
  items->Js.Dict.entries->M.fromArray->M.mapU(decode);

let getItems = ({store, decode, _}, ~keys) =>
  GetItemsJs.dictFromArray(store, keys)
  |> Js.Promise.then_(items =>
       parseItems(decode, items) |> Js.Promise.resolve
     );

let getAllItems = ({store, decode, _}) =>
  GetItemsJs.allDict(store)
  |> Js.Promise.then_(items =>
       parseItems(decode, items) |> Js.Promise.resolve
     );

let setItems = ({store, encode, _}, ~items) => {
  items->M.mapU(encode)
  |> M.toArray
  |> Js.Dict.fromArray
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
