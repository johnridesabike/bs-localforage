type t('a, 'identity);

let make: (Config.t, Id.encodable('a, 'identity)) => t('a, 'identity);

let get: t('a, 'identity) => Js.Promise.t('a);

let set: (t('a, 'identity), ~items: 'a) => Js.Promise.t(unit);
