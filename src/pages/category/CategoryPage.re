Import.style("./style.styl");

open Response;

type data = Shape.responseList(Shape.wechat);

type state = {
  category: option(Shape.category),
  data: option(data),
  loading: bool,
};

type action =
  | Loading
  | UpdateWechat(int, Shape.wechat)
  | CategoryLoaded(Shape.category)
  | ListLoaded(data);

let component = React.reducerComponent("AppCategory");

let fetchCategory = cb =>
  switch (Util.Url.getQuery("id")) {
  | "" => ()
  | id => Api.fetchCategory(id, cb)
  };

let updateTitle = title =>
  Webapi.Dom.(
    switch (title, Document.asHtmlDocument(document)) {
    | ("", None) => ()
    | (title, Some(htmlDoc)) =>
      HtmlDocument.setTitle(htmlDoc, {j|小众订阅 · $title|j})
    | _ => ()
    }
  );

let fetchData =
    (category: option(Shape.category), data: option(data), cb, ()) => {
  let (start, list) =
    switch (data) {
    | None => (0, [||])
    | Some(d) => (d.next, d.list)
    };

  switch (category) {
  | None => ()
  | Some(c) =>
    Api.fetchCategoryWechatList(c.name, start, res =>
      cb({...res, list: Array.concat([list, res.list])})
    )
  };
};

let renderWechat = (idx, data: Shape.wechat, ~send) =>
  <Wechat data key={data.id} onUpdate={w => send(UpdateWechat(idx, w))} />;

let make = _children => {
  ...component,

  initialState: () => {category: None, loading: true, data: None},

  reducer: (action, state) =>
    switch (action) {
    | Loading => React.Update({...state, loading: true})
    | UpdateWechat(idx, wechat) =>
      switch (state.data) {
      | Some(data) =>
        data.list[idx] = wechat;
        React.Update({...state, data: Some(data)});
      | None => React.NoUpdate
      }
    | CategoryLoaded(c) =>
      React.UpdateWithSideEffects(
        {...state, category: Some(c)},
        (
          ({send}) => {
            updateTitle(c.name);
            fetchData(
              Some(c),
              state.data,
              res => send(ListLoaded(res)),
              (),
            );
            send(Loading);
          }
        ),
      )
    | ListLoaded(data) =>
      React.Update({...state, data: Some(data), loading: false})
    },

  didMount: self => fetchCategory(res => self.send(CategoryLoaded(res))),

  render: self => {
    let {category, data} = self.state;

    let onEndReached = () => {
      fetchData(category, data, res => self.send(ListLoaded(res)), ());
      self.send(Loading);
    };

    <ListView
      key="category-wechat-list"
      id="category-wechat-list"
      className="category-wechat-list"
      restoreScrollPosition=false
      loading={self.state.loading}
      data={self.state.data}
      renderItem={renderWechat(~send=self.send)}
      onEndReached
    />;
  },
};