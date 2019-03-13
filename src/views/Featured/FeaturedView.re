Import.style("./style.styl");

open Response;

type data = Shape.responseList(Shape.featured);

type state = {
  data: option(data),
  loading: bool,
};

type action =
  | Refresh
  | Loading
  | Loaded(data);

let component = React.reducerComponent("FeaturedView");

let rec fetchData = (~send, ~refresh=false, ~data: option(data)=None, ()) => {
  let (start, list) =
    switch (data) {
    | None => (0, [||])
    | Some(data) => (data.next, data.list)
    };

  if (refresh) {
    Api.fetchFeaturedList(
      start,
      res => {
        let res = {...res, list: Array.concat([list, res.list])};
        if (refresh && Array.length(res.list) > 0) {
          Storage.setFeaturedList(res);
        };
        send(Loaded(res));
      },
    );
    send(Loading);
  } else {
    Storage.getFeaturedList(data =>
      switch (data) {
      | Some(d) => send(Loaded(d))
      | None => fetchData(~send, ~refresh=true, ())
      }
    );
  };
};

let listViewTime = ListView.makeRenderTime();

let renderItem = (idx, data: Shape.featured) =>
  <div key={data.id}>
    {listViewTime.render(idx, data.createdAt)}
    <FeaturedCard data />
  </div>;

let make = _children => {
  ...component,

  initialState: () => {data: None, loading: false},

  reducer: (action, state) =>
    switch (action) {
    | Refresh =>
      React.UpdateWithSideEffects(
        {data: None, loading: true},
        (
          ({send}) => {
            Storage.clearFeaturedList();
            fetchData(~send, ~refresh=true, ());
          }
        ),
      )
    | Loading => React.Update({...state, loading: true})
    | Loaded(data) => React.Update({data: Some(data), loading: false})
    },

  willUpdate: _ => listViewTime.clean(),

  didMount: self => {
    fetchData(~send=self.send, ());

    let unsubscribe = PubSub.subscribe(RefreshList, _ => self.send(Refresh));
    self.onUnmount(unsubscribe);
  },

  render: self =>
    <ListView
      key="featured-list"
      id="featured-list"
      className="featured-list"
      renderItem
      data={self.state.data}
      loading={self.state.loading}
      onEndReached={
        fetchData(~send=self.send, ~refresh=true, ~data=self.state.data)
      }
    />,
};