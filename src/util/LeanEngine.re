open Util;

module LCConstant = {
  let appId = lcAppId;
  let appKey = lcAppKey;
  let apiVersion = "1.0";

  let apiPrefix = {
    let baseUrl =
      isDev ? "http://localhost:3001" : "https://vrpycq26.engine.lncld.net";
    baseUrl ++ "/1.1/functions/api";
  };
};

type className =
  | WechatMedia
  | Article;

module LCClass = {
  let stringOfClassname = cls =>
    switch (cls) {
    | WechatMedia => "WechatMedia"
    | Article => "Article"
    };

  let url = (cls: className) => "/classes/" ++ stringOfClassname(cls);

  let listUrl = (cls: className, start: int, limit: int) =>
    Url.stringify(
      url(cls),
      [("skip", string_of_int(start)), ("limit", string_of_int(limit))],
    );
};

type apiMethod =
  | FetchArticleContent
  | FetchWechatList
  | FetchWechatArticleList
  | FetchFavoriteList
  | FetchFeaturedList
  | FetchSubscriptionList
  | FetchCategory
  | FetchCategoryList
  | FetchCategoryWechatList
  | FetchWechat
  | CreateSubscription
  | RemoveSubscription
  | CreateFavorite
  | RemoveFavorite;

module LCApi = {
  let stringOfAPIMethod = m =>
    switch (m) {
    | FetchArticleContent => "fetch.article-detail"
    | FetchFavoriteList => "fetch.favorite.list"
    | FetchWechatList => "fetch.recommend-wechat.list"
    | FetchWechatArticleList => "fetch.wechat-article.list"
    | FetchFeaturedList => "fetch.featured.list"
    | FetchSubscriptionList => "fetch.subscription.list"
    | FetchCategory => "fetch.category"
    | FetchCategoryList => "fetch.category.list"
    | FetchCategoryWechatList => "fetch.category-wechat.list"
    | FetchWechat => "fetch.wechat"
    | CreateSubscription => "create.subscription"
    | RemoveSubscription => "remove.subscription"
    | CreateFavorite => "create.favorite"
    | RemoveFavorite => "remove.favorite"
    };

  let url = (m, params) =>
    Url.stringify(
      LCConstant.apiPrefix,
      [("method", stringOfAPIMethod(m)), ...params],
    );
};
