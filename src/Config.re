type driver;
type t;

[@bs.obj]
external make:
  (
    ~description: string=?,
    ~driver: array(driver)=?,
    ~name: string,
    ~size: int=?,
    ~storeName: string,
    ~version: float=?,
    unit
  ) =>
  t;
