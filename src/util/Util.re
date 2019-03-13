[@bs.val] external isDev: bool = "process.env.isDev";
[@bs.val] external lcAppId: string = "process.env.LC_APP_ID";
[@bs.val] external lcAppKey: string = "process.env.LC_APP_KEY";
[@bs.val] [@bs.scope ("window", "navigator")]
external ua: string = "userAgent";

module Url = {
  open Js.String;

  /* TODO: transform to js-friendly code, please */
  let stringify = (base: string, params: list((string, string))): string => {
    let rec join = a =>
      switch (a) {
      | [] => ""
      | [(k, v)] => k ++ "=" ++ v
      | [(k, v), ...rest] => k ++ "=" ++ v ++ "&" ++ join(rest)
      };

    let s = join(params);
    String.length(s) == 0 ? base : base ++ "?" ++ join(params);
  };

  let get = (str, key) =>
    switch (str |> split(key ++ "=")) {
    | [|"", v|] => v
    | _ => ""
    };

  let rec find = (l, key) =>
    switch (l) {
    | [] => ""
    | [one] => get(one, key)
    | [one, ...rest] =>
      let t = get(one, key);
      length(t) === 0 ? find(rest, key) : t;
    };

  let getQuery = key => {
    let url = Router.dangerouslyGetInitialUrl();
    let queryList = Array.to_list(url.search |> split("&"));
    find(queryList, key);
  };

  let host = isDev ? "http://localhost:9000" : "https://wx.whenpigsfly.cn";

  let devTransformUrl = url =>
    isDev ?
      Js.String.replace(
        "https://wx.whenpigsfly.cn",
        "http://localhost:9000",
        url,
      ) :
      url;
};

/* thanks to reason-maze */
let throttle = (fn, time) => {
  let last = ref(None);
  v => {
    switch (last^) {
    | Some(t) => Js.Global.clearTimeout(t)
    | None => ()
    };
    last := Some(Js.Global.setTimeout(() => fn(v), time));
  };
};

let distanceFromBottom: Dom.element => int =
  el => {
    open Webapi.Dom;

    let clientHeight = el |> Element.clientHeight;
    let scrollTop = el |> Element.scrollTop |> int_of_float;
    let scrollHeight = el |> Element.scrollHeight;

    scrollHeight - (clientHeight + scrollTop);
  };

type os =
  | IOS
  | Android;

let os: unit => os =
  () => {
    open Js.String;
    let ua = toLocaleLowerCase(ua);

    if (includes("ios", ua) || includes("iphone", ua)) {
      IOS;
    } else {
      Android;
    };
  };

let easeInOutQuad = (t, b, c, d) => {
  let t = t *. 2.0 /. d;

  if (t < 1.0) {
    c /. 2.0 *. t *. t +. b;
  } else {
    let t = t -. 1.0;
    (-1.0) *. c /. 2.0 *. (t *. (t -. 2.0) -. 1.0) +. b;
  };
};

let scrollTo = (el, toPos, duration) => {
  open Webapi.Dom;

  let duration = float_of_int(duration);
  let toPos = float_of_int(toPos);

  let start = el |> Element.scrollTop;
  let currentTime = ref(0.0);
  let increment = 20.0;

  let rec animateScroll = () => {
    currentTime := currentTime^ +. increment;
    let nextPos =
      easeInOutQuad(currentTime^, start, toPos -. start, duration);

    Element.setScrollTop(el, nextPos);

    if (currentTime^ < duration) {
      let timerId =
        Js.Global.setTimeout(animateScroll, int_of_float(increment));
      ();
    };
  };

  animateScroll();
};
