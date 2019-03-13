/* open Dom.Storage; */

open Response;

type storageKey =
  | SubscriptionList
  | FeaturedList
  | FavoriteList
  | CategoryList
  | SessionToken;

let stringOfStorageKey = k =>
  switch (k) {
  | SubscriptionList => "subscription-list"
  | FeaturedList => "featured-list"
  | FavoriteList => "favorite-list"
  | CategoryList => "category-list"
  | SessionToken => "__session_token__"
  };

let store: Js.Dict.t(string) = Js.Dict.empty();

let get = (key, decoder, cb) => {
  /* let jsonString = localStorage |> getItem(stringOfStorageKey(key)); */
  let jsonString = Js.Dict.get(store, stringOfStorageKey(key));

  switch (jsonString) {
  | Some(s) when s != "" =>
    switch (Json.parse(s)) {
    | Some(s) => s |> decoder |> (res => cb(Some(res)))
    | None => cb(None)
    }
  | _ => cb(None)
  };
};

let set = (key, data) =>
  Js.Dict.set(
    store,
    stringOfStorageKey(key),
    data == Js.Json.null ? "" : Json.stringify(data),
  );
/* localStorage |> setItem(stringOfStorageKey(key), Json.stringify(data)); */

/* let clear = key => localStorage |> removeItem(stringOfStorageKey(key)); */
let clear = key => set(key, Js.Json.null);

let getFeaturedList = cb => get(FeaturedList, Decode.featuredList, cb);
let setFeaturedList = data => set(FeaturedList, Encode.featuredList(data));
let clearFeaturedList = () => clear(FeaturedList);

let getSubscriptionList = cb => get(SubscriptionList, Decode.articleList, cb);
let setSubscriptionList = data =>
  set(SubscriptionList, Encode.articleList(data));
let clearSubscriptionList = () => clear(SubscriptionList);

let setFavoriteList = data => set(FavoriteList, Encode.articleList(data));
let getFavoriteList = cb => get(FavoriteList, Decode.articleList, cb);
let clearFavoriteList = () => clear(FavoriteList);

let setCategoryList = data => set(CategoryList, Encode.categoryList(data));
let getCategoryList = cb => get(CategoryList, Decode.categoryList, cb);
let clearCategoryList = () => clear(CategoryList);

let setToken = token => set(SessionToken, token);
let getToken = () => get(SessionToken);