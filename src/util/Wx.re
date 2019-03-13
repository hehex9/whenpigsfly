[@bs.val] [@bs.scope "window"] external wxEnv: string = "__wxjs_environment";

[@bs.val] [@bs.scope ("window", "wx", "miniProgram")]
external wxNavigateTo: Js.Dict.t(string) => unit = "navigateTo";

let isMiniProgram = () => wxEnv == "miniprogram";

let navigateToWxPage = (page: string, url: string) => {
  let path = "/pages/" ++ page ++ "/" ++ page;
  let url =
    String.length(url) > 0 ?
      path ++ "?url=" ++ Js.Global.encodeURIComponent(url) : path;
  let dict = Js.Dict.fromList([("url", url)]);

  Util.isDev ? Js.log2("navigate to:", url) : ();

  wxNavigateTo(dict);
};

let makeUrl = (page: string, args: option(list((string, string)))) => {
  let tokenPair = ("token", Api.sessionKey^);
  let args =
    switch (args) {
    | Some(p) => [tokenPair, ...p]
    | None => [tokenPair]
    };

  Util.Url.(stringify(host ++ "/" ++ page ++ ".html", args));
};

let navigateTo = (page: string, args: option(list((string, string)))) => {
  let url = makeUrl(page, args);
  Js.log(url);

  switch (page, isMiniProgram()) {
  | ("auth", true) => navigateToWxPage("auth", url)
  | ("auth", false) => Js.log("navigate to auth")
  | (page, true) => navigateToWxPage(page, url)
  | (_, false) => Webapi.Dom.(Location.setHref(location, url))
  };
};

let toAuth = () => navigateTo("auth", None);

let toArticle = id => navigateTo("article", Some([("id", id)]));

let toCategory = id => navigateTo("category", Some([("id", id)]));

let toWechat = id => navigateTo("wechat", Some([("id", id)]));