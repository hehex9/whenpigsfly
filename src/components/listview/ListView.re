Import.style("./style.styl");

type state = {refListView: ref(option(Dom.element))};

type action =
  | Scroll;

let component = React.reducerComponent("ListView");

let scrollPosition = Js.Dict.empty();
let storeScrollPosition = (el, key) =>
  Js.Dict.set(
    scrollPosition,
    key,
    el |> Webapi.Dom.Element.scrollTop |> int_of_float,
  );

let setRef = (r, {React.state}) =>
  state.refListView := Js.Nullable.toOption(r);

let make =
    (
      ~id="",
      ~data: option(Response.Shape.responseList('a)),
      ~loading,
      ~onEndReached=?,
      ~onEndReachedThreshold=30,
      ~restoreScrollPosition=true,
      ~showEndReachedMessage=true,
      ~renderItem,
      ~renderHeader=() => React.null,
      ~renderEmpty=() => React.null,
      ~className="",
      _children,
    ) => {
  ...component,

  initialState: () => {refListView: ref(None)},

  reducer: (action, _state) =>
    switch (action) {
    | Scroll =>
      React.SideEffects(
        (
          self =>
            switch (self.state.refListView^, onEndReached, data) {
            | (Some(listRef), Some(fn), Some(data)) =>
              if (restoreScrollPosition) {
                storeScrollPosition(listRef, id);
              };

              if (!loading
                  && !data.isEnd
                  && Util.distanceFromBottom(listRef) < onEndReachedThreshold) {
                fn();
              };
            | _ => ()
            }
        ),
      )
    },

  didMount: self =>
    switch (onEndReached, self.state.refListView^) {
    | (Some(_), Some(r)) =>
      open Webapi.Dom;
      let handler = Util.throttle(_e => self.send(Scroll), 300);
      let target = Element.asEventTarget(r);
      EventTarget.addEventListener("scroll", handler, target);

      self.onUnmount(
        PubSub.subscribe(RefreshList, _ =>
          Js.Dict.set(scrollPosition, id, 0)
        ),
      );

      /* TODO: support multi listview */
      self.onUnmount(
        PubSub.subscribe(ScrollToTop, _ => Util.scrollTo(r, 0, 800)),
      );

      self.onUnmount(() =>
        EventTarget.removeEventListener("scroll", handler, target)
      );
    | _ => ()
    },

  didUpdate: ({oldSelf}) =>
    if (restoreScrollPosition) {
      /* TODO: fuck me (wrong lifecycle, not didUpdate) */

      let scrollTop = Js.Dict.get(scrollPosition, id);

      switch (scrollTop, oldSelf.state.refListView^) {
      | (Some(t), Some(el)) =>
        Webapi.Dom.Element.setScrollTop(el, float_of_int(t))
      | _ => ()
      };
    },

  render: self => {
    let (items, len, isEnd) =
      switch (data) {
      | None => ([||], 0, false)
      | Some(data) => (
          Array.mapi(renderItem, data.list),
          Array.length(data.list),
          data.isEnd,
        )
      };

    <div className={"list " ++ className} ref={self.handle(setRef)}>
      {renderHeader()}
      {
        switch (items, loading) {
        | ([||], false) => renderEmpty()
        | ([||], true) when len == 0 => <Spinner loading=true />
        | _ =>
          <>
            {React.array(items)}
            {
              showEndReachedMessage ?
                <div
                  className={
                    "list-loading"
                    ++ (loading ? " active" : "")
                    ++ (isEnd ? " disabled" : "")
                  }>
                  {
                    loading ?
                      React.string({js|加载中...|js}) :
                      isEnd ? React.string({js|- 没了 -|js}) : React.null
                  }
                </div> :
                React.null
            }
          </>
        }
      }
    </div>;
  },
};

type makeRenderTime = {
  clean: unit => unit,
  render: (int, int) => ReasonReact.reactElement,
};

let makeRenderTime: unit => makeRenderTime =
  () => {
    let timeSet = ref(Type.StringSet.empty);

    let renderTime = (idx, timestamp) => {
      let t = Text.showDate(timestamp);

      switch (Type.StringSet.find(t, timeSet^)) {
      | _ => React.null
      | exception Not_found when t == {js|今天|js} => React.null
      | exception Not_found =>
        timeSet := Type.StringSet.add(t, timeSet^);
        <div className={"time" ++ (idx == 0 ? " first" : "")}>
          {React.string(t)}
        </div>;
      };
    };

    {clean: () => timeSet := Type.StringSet.empty, render: renderTime};
  };