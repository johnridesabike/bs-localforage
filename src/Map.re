open Belt.Result;
module M = Belt.Map.String;
module LF = LocalForageJs;
include LoadAllPlugins;

let errorHandler = error => {
  Js.Console.error(error);
  Js.String.make(error);
};

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
  ->FutureJs.fromPromise(errorHandler)
  ->Future.flatMapOk(value =>
      switch (value->Js.Nullable.toOption->Belt.Option.map(decode)) {
      | Some(value) => Future.value(Ok(Some(value)))
      | None => Future.value(Ok(None))
      | exception error =>
        Js.Console.error(error);
        Future.value(Error(Js.String.make(error)));
      }
    );
};
let setItem = ({store, encode}, ~key, ~v) =>
  LF.setItem(store, key, encode(v));
let getKeys = ({store}) =>
  LF.keys(store)->FutureJs.fromPromise(Js.Console.error);
let _parseItems = (decode, items) =>
  items->Js.Dict.entries->M.fromArray->M.map(decode);
let getItems = ({store, decode}, ~keys) =>
  GetItemsJs.dictFromArray(store, keys)
  ->FutureJs.fromPromise(errorHandler)
  ->Future.flatMapOk(items =>
      switch (_parseItems(decode, items)) {
      | exception error =>
        Js.Console.error(error);
        Future.value(Error(Js.String.make(error)));
      | result => Future.value(Ok(result))
      }
    );
let getAllItems = ({store, decode}) =>
  GetItemsJs.allDict(store)
  ->FutureJs.fromPromise(errorHandler)
  ->Future.flatMapOk(items =>
      switch (_parseItems(decode, items)) {
      | exception error =>
        Js.Console.error(error);
        Future.value(Error(Js.String.make(error)));
      | result => Future.value(Ok(result))
      }
    );
let setItems = ({store, encode}, ~items) => {
  switch (M.map(items, encode)) {
  | result =>
    result
    ->M.toArray
    ->Js.Dict.fromArray
    ->SetItemsJs.fromDict(store, _)
    ->FutureJs.fromPromise(errorHandler)
  | exception error =>
    Js.Console.error(error);
    Future.value(Error(Js.String.make(error)));
  };
};
let removeItems = ({store}, ~items) =>
  RemoveItemsJs.fromArray(store, items)
  ->FutureJs.fromPromise(errorHandler);
let iterate = ({store, decode}, ~f) => {
  LF.iterate(store, (. value, key, iterationNumber) =>
    switch (decode(value)) {
    | result => f(. result, key, iterationNumber)
    | exception error => Js.Console.error(error)
    }
  )
  ->FutureJs.fromPromise(errorHandler);
};