# bs-localforage

This is a binding for [LocalForage](https://localforage.github.io/localForage/)
for BuckleScript and Reason. I built it to use within my app
[Coronate](https://github.com/johnridesabike/coronate). It works well for my 
purposes, but it's definitely in an alpha stage. The API isn't complete, and
will probably change unexpectedly.

## Installation

run the command:
```sh
npm i @johnridesabike/bs-localforage
```

and add these to your `bsDependencies` in `bsconfig.json`:
```json
"bs-dependencies": [
    "@johnridesabike/bs-localforage"
]
```

## Documentation

For now, I recommend viewing the files in `src` for the type definitions, which 
should be self-explanatory. 

Here is some additional information which may help you understand them:

### LocalForageJs

This is a zero-overhead binding to the LocalForage API. It's simple and not very
type-safe. It types all of the input and output as `Js.Json.t`.

`createInstance` from LocalForage is bound to `make` in Reason for consistency
with other Reason libraries. Its configuration object is created with the
`config` function.

One key difference between this and the JavaScript LocalForage is that the JS
version exports a default instance. In this Reason version, you have to call
`make` to use your own instance first.

LocalForage is great because it's simple, but that simplicity allows for lots
of unsafe code. To address that, we have `Map` and `Record`:

### Id

Before creating a `Map` or a `Record`, you need to encode your data with the
`Id.MakeEncodable` functor:

```reason
module MyEncodable = LocalForage.Id.MakeEncodable({
  type t = string;
  let encode = Js.Json.string;
  let decode = Js.Json.decodeString;
};
```

### Map

This component uses LocalForage in a "mapping" fashion idiomatic to Reason. Its 
purpose is storing a series of objects, for example, chess player profiles.

To create an instance for it, you have to call `Map.make`. It takes a `config`
object and an encode module created by `Id.MakeEncodable`.

Any time you use a `Map` function on an instance, it will use the `encode` and 
`decode` functions to convert the JSON to your type. 

`Map` returns data in an `array((string, 'value))` to be easily imported into
your map container of choice.

### Record

This component treats a LocalForage store like a Reason record. Its purpose is 
storing data with a set number of fields, such as a configuration file.

To create an instance for it, you have to call `Record.make`. It takes a
`config` object and an encode module created by `Id.MakeEncodable`.

### GetItemsJs, RemoveItemsJs, SetItemsJs, & LoadAllPlugins

These are bindings to LocalForage plugins of the same names. Each one has a
`load` function which must be called before using them. For convenience (and so
`load` doesn't get called more than necessary) `LoadAllPlugins.re` will load
them all if it's included in your project. `Map` and `Record` both include it
automatically.

## Example code

[See Coronate's `Db` module](https://github.com/johnridesabike/coronate/blob/master/src/Db.re)

## Changelog

### 2020-04-09
- Added `Id` module.
- `Map` and `Record` now require a module created by `Id.MakeEncodable`.

### 2020-3-28
- `Map` no longer returns a `Belt.Map`. It now returns `array((key, value))`
  for better compatibility across libraries.

### 2020-3-20
- Improved performanced.
- Added `Map.iterateU` to compliment `Map.iterate`. `iterateU` takes an
  uncurried callback.
- Moved `Config` module out of `LocalForageJs.Config`.
- **Exceptions are not automaticaly caught**. For better flexibility, you can
  handle rejected promises on the user side now.

### 2019-12-11
- Updated to BuckleScript 7 and reorganized some of the functions.

### 2019-09-12
- Removed the reason-future dependency. It now only returns JS promises.

## Development info

Q: "Why does this do *a* instead of *b*?"
A: Probably because that's what I'm using in the Coronate app, so it made sense
to be consistent here. Likewise, I haven't implemented features that aren't
relevant to my project yet. 

If you think bs-localforage can be improved, I'm happy to recieve your issues or
pull requests. Feel free to fork this code and develop your own vesion, too!

## Development commands

### Build
```
npm run build
```

### Watch

```
npm run watch
```

