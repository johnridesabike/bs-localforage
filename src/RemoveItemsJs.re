type t = LocalForageJs.t;
[@bs.module] external localForage: t = "localforage";
[@bs.module "localforage-removeitems"]
external extendPrototype: t => unit = "extendPrototype";
let load = () => extendPrototype(localForage);
[@bs.send]
external fromArray: (t, array(string)) => Js.Promise.t(unit) = "removeItems";