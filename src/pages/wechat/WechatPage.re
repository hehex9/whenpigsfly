Import.style("./style.styl");

open Response;

type data = Shape.responseList(Shape.article);

type state = {
  loading: bool,
  wechat: option(Shape.wechat),
  data: option(data),
};

type action =
  | WechatLoaded(option(Shape.wechat))
  | DataLoaded(data)
  | UpdateArticle(int, Shape.article)
  | UpdateWechat(Shape.wechat)
  | Loading;

let component = React.reducerComponent("WechatPage");

let fetchData = (wechat: option(Shape.wechat), data: option(data), cb, ()) => {
  let (start, list) =
    switch (data) {
    | None => (0, [||])
    | Some(d) => (d.next, d.list)
    };

  switch (wechat) {
  | None => ()
  | Some(w) =>
    Api.fetchWechatArticleList(w.id, start, res =>
      cb({...res, list: Array.concat([list, res.list])})
    )
  };
};

let onFavArticle = (a: Shape.article, afterAction) => {
  a.isFavorited ?
    Api.removeFavorite(a.id, _ => afterAction({...a, isFavorited: false})) :
    Api.createFavorite(a.id, _ => afterAction({...a, isFavorited: true}));
  Storage.clearFavoriteList();
};

let headerPlaceholder =
  <div className="wechat-header-placholder">
    <div className="wechat-header-avatar" />
  </div>;

let renderArticle = (idx, data: Shape.article, ~send) =>
  <Article
    data
    key={data.id}
    showAction=false
    showHeart=true
    showTime=true
    onAction={_e => onFavArticle(data, a => send(UpdateArticle(idx, a)))}
  />;

let make = _children => {
  ...component,

  initialState: () => {loading: true, wechat: None, data: None},

  reducer: (action, state) =>
    switch (action) {
    | Loading => React.Update({...state, loading: true})
    | WechatLoaded(wechat) =>
      React.UpdateWithSideEffects(
        {...state, wechat},
        (
          ({send}) => {
            fetchData(wechat, state.data, res => send(DataLoaded(res)), ());
            send(Loading);
          }
        ),
      )
    | DataLoaded(data) =>
      React.Update({...state, loading: false, data: Some(data)})
    | UpdateArticle(idx, article) =>
      switch (state.data) {
      | Some(data) =>
        data.list[idx] = article;
        React.Update({...state, data: Some(data)});
      | None => React.NoUpdate
      }
    | UpdateWechat(wechat) => React.Update({...state, wechat: Some(wechat)})
    },

  didMount: self => {
    let id = Util.Url.getQuery("id");
    switch (id) {
    | "" => self.send(WechatLoaded(None))
    | id => Api.fetchWechat(id, res => self.send(WechatLoaded(Some(res))))
    };
  },

  render: self => {
    let {wechat, data, loading} = self.state;

    let onEndReached = () => {
      fetchData(wechat, data, res => self.send(DataLoaded(res)), ());
      self.send(Loading);
    };

    let renderHeader = () =>
      switch (wechat) {
      | None => headerPlaceholder
      | Some(w) =>
        <div className="wechat-header">
          <div className="wechat-header-info">
            <img src={w.avatar} alt="" />
            <strong> {React.string(w.name)} </strong>
            <WechatFoButton
              data=w
              onUpdate=(w => self.send(UpdateWechat(w)))
            />
          </div>
          <p> {React.string(w.description)} </p>
        </div>
      };

    <ListView
      key="wechat-article-list"
      className="wechat-article-list"
      id="wechat-article-list"
      data
      loading
      onEndReached
      renderHeader
      renderItem={renderArticle(~send=self.send)}
    />;
  },
};