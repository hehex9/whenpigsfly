Import.style("./style.styl");

open Response;

type data = Shape.responseList(Shape.article);

type state = {
  data: option(data),
  loading: bool,
};

type action =
  | Refresh
  | Loading
  | Loaded(data)
  | UpdateArticle(int, Shape.article);

let component = React.reducerComponent("Favorites");

let renderEmpty = () =>
  <Empty> <IconHeart /> <p> {React.string({js|空|js})} </p> </Empty>;

let rec fetchData = (~send, ~refresh=false, ~data: option(data)=None, ()) => {
  let (start, list) =
    switch (data) {
    | None => (0, [||])
    | Some(d) => (d.next, d.list)
    };

  if (refresh) {
    Api.fetchFavoriteList(
      start,
      res => {
        let res = {...res, list: Array.concat([list, res.list])};
        if (refresh && Array.length(res.list) > 0) {
          Storage.setFavoriteList(res);
        };
        send(Loaded(res));
      },
    );
    send(Loading);
  } else {
    Storage.getFavoriteList(data =>
      switch (data) {
      | Some(d) =>
        Js.log2("hahahaha", d);
        send(Loaded(d));
      | None => fetchData(~send, ~refresh=true, ~data, ())
      }
    );
  };
};

let onFavArticle = (a: Shape.article, afterAction) => {
  a.isFavorited ?
    Api.removeFavorite(a.id, _ => afterAction({...a, isFavorited: false})) :
    Api.createFavorite(a.id, _ => afterAction({...a, isFavorited: true}));
  Storage.clearFavoriteList();
};

let renderArticle = (idx, data: Shape.article, ~send) =>
  <Article
    data
    key={data.id}
    onAction={_e => onFavArticle(data, a => send(UpdateArticle(idx, a)))}
    showHeart=true
    showAction=false
  />;

let make = _children => {
  ...component,

  initialState: () => {loading: false, data: None},

  reducer: (action, state) =>
    switch (action) {
    | Refresh =>
      React.UpdateWithSideEffects(
        {loading: true, data: None},
        (
          ({send}) => {
            Storage.clearFavoriteList();
            fetchData(~send, ~refresh=true, ());
          }
        ),
      )
    | Loading => React.Update({...state, loading: true})
    | Loaded(d) => React.Update({loading: false, data: Some(d)})
    | UpdateArticle(idx, article) =>
      switch (state.data) {
      | Some(data) =>
        data.list[idx] = article;
        React.Update({...state, data: Some(data)});
      | None => React.NoUpdate
      }
    },

  didMount: self => {
    fetchData(~send=self.send, ());

    self.onUnmount(PubSub.subscribe(RefreshList, _ => self.send(Refresh)));
  },

  render: self =>
    <ListView
      key="favorites"
      id="favorites-list"
      className="favorites-list"
      data={self.state.data}
      loading={self.state.loading}
      onEndReached={
        fetchData(~send=self.send, ~data=self.state.data, ~refresh=true)
      }
      renderItem={renderArticle(~send=self.send)}
      renderEmpty
    />,
};