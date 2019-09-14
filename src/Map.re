module M = Belt.Map.String;
module LF = LocalForageJs;
include LoadAllPlugins;
type t('a) = {
  store: LocalForageJs.t,
  decode: Js.Json.t => 'a,
  encode: 'a => Js.Json.t,
};
type config = LocalForageJs.config;
module type Data = {
  type t;
  let decode: Js.Json.t => t;
  let encode: t => Js.Json.t;
};
let make = (config, type t, data: (module Data with type t = t)) => {
  module Data = (val data);
  {store: LF.make(config), encode: Data.encode, decode: Data.decode};
};
let getItem = ({store, decode}, ~key) => {
  LF.getItem(store, key)
  |> Js.Promise.then_(value =>
       switch (value |> Js.Nullable.toOption |> Belt.Option.map(_, decode)) {
       | Some(value) => Js.Promise.resolve(Some(value))
       | None => Js.Promise.resolve(None)
       | exception error =>
         Js.Console.error(error);
         Js.Promise.reject(error);
       }
     );
};
let setItem = ({store, encode}, ~key, ~v) =>
  LF.setItem(store, key, encode(v));
let getKeys = ({store}) => LF.keys(store);
let parseItems = (decode, items) =>
  items |> Js.Dict.entries |> M.fromArray |> M.map(_, decode);
let getItems = ({store, decode}, ~keys) =>
  GetItemsJs.dictFromArray(store, keys)
  |> Js.Promise.then_(items =>
       switch (parseItems(decode, items)) {
       | exception error =>
         Js.Console.error(error);
         Js.Promise.reject(error);
       | result => Js.Promise.resolve(result)
       }
     );
let getAllItems = ({store, decode}) =>
  GetItemsJs.allDict(store)
  |> Js.Promise.then_(items =>
       switch (parseItems(decode, items)) {
       | exception error =>
         Js.Console.error(error);
         Js.Promise.reject(error);
       | result => Js.Promise.resolve(result)
       }
     );
let setItems = ({store, encode}, ~items) => {
  items
  |> M.map(_, encode)
  |> M.toArray
  |> Js.Dict.fromArray
  |> SetItemsJs.fromDict(store);
};
let removeItems = ({store}, ~items) =>
  RemoveItemsJs.fromArray(store, items);
let iterate = ({store, decode}, ~f) => {
  LF.iterate(store, (. value, key, iterationNumber) =>
    switch (decode(value)) {
    | result => f(. result, key, iterationNumber)
    | exception error => Js.Console.error(error)
    }
  );
};