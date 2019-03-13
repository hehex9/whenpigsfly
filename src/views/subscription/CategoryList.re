open Response;

type data = Shape.responseList(Shape.category);

type state = {
  data: option(data),
  loading: bool,
};

type action =
  | Refresh
  | OpenCategory(string)
  | Loading
  | Loaded(data);

let component = React.reducerComponent("CategoryList");

let rec fetchData = (~send, ~refresh=false, ~data: option(data)=None, ()) => {
  let (start, list) =
    switch (data) {
    | None => (0, [||])
    | Some(d) => (d.next, d.list)
    };

  if (refresh) {
    Api.fetchCategoryList(
      start,
      res => {
        let res = {...res, list: Array.concat([list, res.list])};
        if (refresh && Array.length(res.list) > 0) {
          Storage.setCategoryList(res);
        };
        send(Loaded(res));
      },
    );
    send(Loading);
  } else {
    Storage.getCategoryList(data =>
      switch (data) {
      | Some(d) => send(Loaded(d))
      | None => fetchData(~send, ~refresh=true, ~data, ())
      }
    );
  };
};

let renderCategory = (_idx, data: Shape.category, ~send) => {
  let cover = data.cover;

  <div
    className="category"
    key={data.id}
    onClick={_e => send(OpenCategory(data.id))}>
    <strong> {React.string(data.name)} </strong>
    <div
      className="cover"
      style={ReactDOMRe.Style.make(~backgroundImage={j|url($cover)|j}, ())}
    />
  </div>;
};

let make = _children => {
  ...component,

  initialState: () => {loading: false, data: None},

  reducer: (action, state) =>
    switch (action) {
    | Refresh =>
      React.UpdateWithSideEffects(
        {data: None, loading: true},
        (({send}) => fetchData(~send, ~refresh=true, ())),
      )
    | OpenCategory(id) =>
      Wx.toCategory(id);
      React.NoUpdate;
    | Loading => React.Update({...state, loading: true})
    | Loaded(data) => React.Update({loading: false, data: Some(data)})
    },

  didMount: self => {
    fetchData(~send=self.send, ());
    self.onUnmount(PubSub.subscribe(RefreshList, _ => self.send(Refresh)));
  },

  render: self =>
    <ListView
      id="category-list"
      className="category-list"
      key="category-list"
      loading={self.state.loading}
      data={self.state.data}
      renderItem={renderCategory(~send=self.send)}
      showEndReachedMessage=false
      onEndReached={
        fetchData(~send=self.send, ~refresh=true, ~data=self.state.data)
      }
    />,
};