open Js.Promise;
open LeanEngine;
open Response;

let sessionKey = ref("");
let headers = () =>
  Fetch.HeadersInit.make({
    "Content-Type": "application/json",
    "X-LC-Id": LCConstant.appId,
    "X-LC-Key": LCConstant.appKey,
    "X-LC-Session": sessionKey^,
  });

let setupToken = () => sessionKey := Util.Url.getQuery("token");

let getJSON = (api: string, method: Fetch.requestMethod, body) => {
  let body = Fetch.BodyInit.make(Js.Json.stringify(Js.Json.object_(body)));

  Fetch.fetchWithInit(
    api,
    Fetch.RequestInit.make(~method_=method, ~headers=headers(), ~body, ()),
  )
  |> then_(Fetch.Response.json);
};

let httpPost = (api: string, data) => getJSON(api, Post, data);

let sign = (v, method, t, token) =>
  Encrypt.md5(
    "v"
    ++ v
    ++ "method"
    ++ method
    ++ "t"
    ++ string_of_int(t)
    ++ "token"
    ++ token,
  );

module Payload = {
  let make = (api, params: list((string, 'a))) => {
    open Js.Json;

    let v = LCConstant.apiVersion;
    let method = LCApi.stringOfAPIMethod(api);
    let t = Js.Date.getTime(Js.Date.make()) /. 1000.0 |> int_of_float;

    Js.Dict.fromList([
      ("t", number(float_of_int(t))),
      ("v", string(v)),
      ("method", string(method)),
      ("sign", string(sign(v, method, t, sessionKey^))),
      ...params,
    ]);
  };

  let int = (k, v) => (k, Js.Json.number(float_of_int(v)));

  let string = (k, v) => (k, Js.Json.string(v));
};

module FetchList = {
  let run = (api, decoder, payload, cb) =>
    httpPost(LCConstant.apiPrefix, Payload.make(api, payload))
    |> then_(json =>
         json
         |> decoder
         |> (
           list => {
             cb(list);
             resolve();
           }
         )
       )
    |> ignore;
};

module Request = {
  let run = (api, decoder, params, cb) =>
    httpPost(LCConstant.apiPrefix, Payload.make(api, params))
    |> then_(json =>
         json
         |> decoder
         |> (
           res => {
             cb(res);
             resolve();
           }
         )
       )
    |> ignore;
};

let fetchFeaturedList = (start, cb) =>
  FetchList.run(
    FetchFeaturedList,
    Decode.featuredListResult,
    [Payload.int("start", start)],
    cb,
  );

let fetchSubscriptionList = (start, cb) =>
  FetchList.run(
    FetchSubscriptionList,
    Decode.articleListResult,
    [Payload.int("start", start)],
    cb,
  );

let fetchWechatList = (start, cb) =>
  FetchList.run(
    FetchWechatList,
    Decode.wechatListResult,
    [Payload.int("start", start)],
    cb,
  );

let fetchFavoriteList = (start, cb) =>
  FetchList.run(
    FetchFavoriteList,
    Decode.articleListResult,
    [Payload.int("start", start)],
    cb,
  );

let fetchCategory = (id, cb) =>
  Request.run(
    FetchCategory,
    Decode.categoryResult,
    [Payload.string("id", id)],
    cb,
  );

let fetchCategoryList = (start, cb) =>
  FetchList.run(
    FetchCategoryList,
    Decode.categoryListResult,
    [Payload.int("start", start)],
    cb,
  );

let fetchCategoryWechatList = (category, start, cb) =>
  FetchList.run(
    FetchCategoryWechatList,
    Decode.wechatListResult,
    [Payload.int("start", start), Payload.string("category", category)],
    cb,
  );

let fetchWechatArticleList = (id, start, cb) =>
  FetchList.run(
    FetchWechatArticleList,
    Decode.articleListResult,
    [Payload.int("start", start), Payload.string("id", id)],
    cb,
  );

let createFavorite = (id, cb) =>
  Request.run(
    CreateFavorite,
    Decode.afterCreate,
    [("id", Js.Json.string(id))],
    cb,
  );
let removeFavorite = (id, cb) =>
  Request.run(
    RemoveFavorite,
    Decode.afterRemove,
    [Payload.string("id", id)],
    cb,
  );
let createSubscription = (id, cb) =>
  Request.run(
    CreateSubscription,
    Decode.afterCreate,
    [Payload.string("id", id)],
    cb,
  );
let removeSubscription = (id, cb) =>
  Request.run(
    RemoveSubscription,
    Decode.afterRemove,
    [Payload.string("id", id)],
    cb,
  );
let fetchArticleContent = (id, cb) =>
  Request.run(
    FetchArticleContent,
    Decode.articleContent,
    [Payload.string("id", id)],
    cb,
  );
let fetchWechat = (id, cb) =>
  Request.run(
    FetchWechat,
    Decode.wechatResult,
    [Payload.string("id", id)],
    cb,
  );