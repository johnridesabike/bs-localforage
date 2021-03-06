type t;

[@bs.module "localforage"] external indexedDb: Config.driver = "INDEXEDDB";
[@bs.module "localforage"] external webSql: Config.driver = "WEBSQL";
[@bs.module "localforage"]
external localStorage: Config.driver = "LOCALSTORAGE";
[@bs.module "localforage"] external make: Config.t => t = "createInstance";

[@bs.module "localforage"]
external clear: unit => Js.Promise.t(unit) = "clear";

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
  (t, [@bs.uncurry] ((Js.Json.t, string, int) => unit)) => Js.Promise.t(unit) =
  "iterate";

/** Settings API */
[@bs.send]
external setDriver: (t, Config.driver) => unit = "setDriver";
[@bs.send]
external setDriverMany: (t, array(Config.driver)) => unit = "setDriver";

/* `config` is mainly used for setting configs, but can also be used for
   getting info */
[@bs.send] external setConfig: (t, Config.t) => Config.t = "config";
[@bs.send] external getConfig: t => Config.t = "config";

/* Driver API: not implemented yet. */
