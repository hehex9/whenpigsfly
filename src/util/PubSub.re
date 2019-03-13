type pubsubEvent =
  | RefreshList
  | ScrollToTop;

let getKey = e =>
  switch (e) {
  | RefreshList => "refresh_list"
  | ScrollToTop => "scroll_to_top"
  };

let store: Js.Dict.t(Js.Dict.t(Js.Dict.t(string) => unit)) =
  Js.Dict.empty();

let publish = (e: pubsubEvent, ~data=?, ()) => {
  let key = getKey(e);
  let dict =
    switch (Js.Dict.get(store, key)) {
    | None => Js.Dict.empty()
    | Some(v) => v
    };

  Array.iter(
    fn =>
      switch (data) {
      | None => fn(Js.Dict.empty())
      | Some(d) => fn(d)
      },
    Js.Dict.values(dict),
  );
};

let id = ref(0);

let subscribe = (e: pubsubEvent, fn: Js.Dict.t(string) => unit) => {
  id := id^ + 1;

  let key = getKey(e);
  let nextId = string_of_int(id^);

  let dict =
    switch (Js.Dict.get(store, key)) {
    | None => Js.Dict.empty()
    | Some(v) => v
    };

  Js.Dict.set(dict, nextId, fn);
  Js.Dict.set(store, key, dict);

  /* TODO: `nextId` should be deleted */
  () => Js.Dict.set(dict, nextId, _ => ());
};