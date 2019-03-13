module Subscription = {
  type text =
    | Subscribe
    | UnSubscribe
    | Fav
    | UnFav;

  let getText = t =>
    switch (t) {
    | Subscribe => {js|订阅|js}
    | UnSubscribe => {js|取消订阅|js}
    | Fav => {js|收藏|js}
    | UnFav => {js|取消收藏|js}
    };
};

let oneDay = 60 * 60 * 24;

let showTime: int => string =
  t => {
    open Js.Date;

    let time = fromFloat(Js.Float.fromString(string_of_int(t) ++ "000"));
    let year = getFullYear(time) |> int_of_float |> string_of_int;
    let month = getMonth(time) +. 1.0 |> int_of_float |> string_of_int;
    let date = getDate(time) |> int_of_float |> string_of_int;

    year ++ "-" ++ month ++ "-" ++ date;
  };

let showDate: int => string =
  t => {
    open Js.Date;

    let time = fromFloat(Js.Float.fromString(string_of_int(t) ++ "000"));
    let year = getFullYear(time) |> int_of_float |> string_of_int;
    let month = getMonth(time) +. 1.0 |> int_of_float |> string_of_int;
    let date = getDate(time) |> int_of_float |> string_of_int;

    let now = make();
    let todayDate = fromFloat(setHours(now, 0.0));
    let todayDate = fromFloat(setMinutes(todayDate, 0.0));
    let todayDate = int_of_float(setSeconds(todayDate, 0.0) /. 1000.0);

    let yearDate = fromFloat(setDate(now, 1.0));
    let yearDate = int_of_float(setMonth(yearDate, 0.0) /. 1000.0);

    switch (todayDate) {
    | today when t >= today => {js|今天|js}
    | today when t >= today - oneDay => {js|昨天|js}
    | today when t >= today - 2 * oneDay => {js|前天|js}
    | today when t >= today - 3 * oneDay => {js|3天前|js}
    | today when t >= today - 4 * oneDay => {js|4天前|js}
    | today when t >= today - 5 * oneDay => {js|5天前|js}
    | today when t >= today - 6 * oneDay => {js|6天前|js}
    | today when t >= today - 2 * 7 * oneDay => {js|一周前|js}
    | _ when t >= yearDate => month ++ {js|月|js} ++ date ++ {js|日|js}
    | _ => year ++ "-" ++ month ++ "-" ++ date
    };
  };

let trim = (~count=20, desc) =>
  String.length(desc) > count ? String.sub(desc, 0, count) ++ "..." : desc;