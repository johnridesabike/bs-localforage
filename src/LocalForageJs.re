type t;
type driver;
[@bs.deriving abstract]
type config = {
  [@bs.optional]
  driver: array(driver),
  name: string,
  [@bs.optional]
  size: int,
  storeName: string,
  [@bs.optional]
  version: float,
  [@bs.optional]
  description: string,
};
[@bs.module "localforage"] external indexedDb: driver = "INDEXEDDB";
[@bs.module "localforage"] external webSql: driver = "WEBSQL";
[@bs.module "localforage"] external localStorage: driver = "LOCALSTORAGE";
[@bs.module "localforage"] 
external make: config => t = "createInstance";

[@bs.module "localforage"]
external clear: unit => unit = "clear";

/** Data API */
[@bs.send]
external setItem: (t, string, Js.Json.t) => Js.Promise.t(unit) = "setItem";
[@bs.send]
external getItem: (t, string) => Js.Promise.t(Js.Nullable.t(Js.Json.t)) =
  "getItem";
[@bs.send]
external removeItem: (t, string) => Js.Promise.t(unit) = "removeItem";
[@bs.send] external length: t => Js.Promise.t(int) = "length";
[@bs.send] external key: t => Js.Promise.t(string) = "key";
[@bs.send] external keys: t => Js.Promise.t(array(string)) = "keys";
[@bs.send]
external iterate:
  (t, (. Js.Json.t, string, int) => unit) => Js.Promise.t(unit) =
  "iterate";

/** Settings API */
[@bs.send]
external setDriver: (t, driver) => unit = "setDriver";
[@bs.send] external setDriverMany: (t, array(driver)) => unit = "setDriver";

/* `config` is mainly used for setting configs, but can also be used for
   getting info */
[@bs.send] external setConfig: (t, config) => config = "config";
[@bs.send] external getConfig: t => config = "config";

/* Driver API: not implemented yet. */
