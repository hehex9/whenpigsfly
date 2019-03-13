open Response;

type data = Shape.responseList(Shape.article);

type state = {
  loading: bool,
  data: option(data),
  articleIdx: option(int),
  toastVisible: bool,
  toastText: string,
};

type action =
  | Refresh
  | Loading
  | Loaded(data)
  | Toast(string)
  | HideToast
  | OpenActionSheet(int)
  | UpdateArticle(int, Shape.article)
  | HideActionSheet;

let component = React.reducerComponent("SubscriptionList");

let rec fetchData = (~send, ~refresh=false, ~data: option(data)=None, ()) => {
  let (start, list) =
    switch (data) {
    | None => (0, [||])
    | Some(d) => (d.next, d.list)
    };

  if (refresh) {
    Api.fetchSubscriptionList(
      start,
      res => {
        let res = {...res, list: Array.concat([list, res.list])};
        if (refresh && Array.length(res.list) > 0) {
          Storage.setSubscriptionList(res);
        };
        send(Loaded(res));
      },
    );
    send(Loading);
  } else {
    Storage.getSubscriptionList(data =>
      switch (data) {
      | Some(d) => send(Loaded(d))
      | None => fetchData(~send, ~refresh=true, ())
      }
    );
  };
};

let listViewTime = ListView.makeRenderTime();

let renderArticle = (idx, data: Shape.article, ~send) =>
  <div key={data.id}>
    {listViewTime.render(idx, data.createdAt)}
    <Article data onAction={_e => send(OpenActionSheet(idx))} />
  </div>;

let renderEmpty = () =>
  <Empty> <IconBookmark /> <p> {React.string({js|空|js})} </p> </Empty>;

let newArticleWithSubscribed = (article: Shape.article, isSubscribed) => {
  ...article,
  wechat: {
    ...article.wechat,
    isSubscribed,
  },
};

let onClickActionSheet =
    (idx, ~article: option(Shape.article), ~toast, ~afterAction) =>
  switch (article, idx) {
  | (Some(a), 0) =>
    a.isFavorited ?
      Api.removeFavorite(a.id, _ => afterAction({...a, isFavorited: false})) :
      Api.createFavorite(
        a.id,
        _ => {
          toast({js|收藏成功|js});
          afterAction({...a, isFavorited: true});
        },
      )
  | (Some(a), 1) =>
    a.wechat.isSubscribed ?
      Api.removeSubscription(a.wechat.id, _ =>
        afterAction(newArticleWithSubscribed(a, false))
      ) :
      Api.createSubscription(a.wechat.id, _ =>
        afterAction(newArticleWithSubscribed(a, true))
      )
  | (Some(a), 2) => Wx.toWechat(a.wechat.id)
  | _ => ()
  };

let actionSheetData = (article: option(Shape.article)) =>
  Text.Subscription.(
    switch (article) {
    | None => [||]
    | Some(a) =>
      let name = a.wechat.name;
      let wechatText = {j|$name|j};
      switch (a.wechat.isSubscribed, a.isFavorited) {
      | (true, true) => [|
          getText(UnFav),
          getText(UnSubscribe),
          wechatText,
        |]
      | (true, false) => [|getText(Fav), getText(UnSubscribe), wechatText|]
      | (false, true) => [|getText(UnFav), getText(Subscribe), wechatText|]
      | (false, false) => [|getText(Fav), getText(Subscribe), wechatText|]
      };
    }
  );

let make = _children => {
  ...component,

  initialState: () => {
    data: None,
    loading: false,
    articleIdx: None,
    toastVisible: false,
    toastText: "",
  },

  reducer: (action, state) =>
    switch (action) {
    | Toast(t) =>
      React.UpdateWithSideEffects(
        {...state, toastVisible: true, toastText: t},
        (
          ({send, onUnmount}) => {
            open Js.Global;
            let timerId = setTimeout(() => send(HideToast), 1200);
            onUnmount(() => clearTimeout(timerId));
          }
        ),
      )
    | HideToast => React.Update({...state, toastVisible: false})
    | Refresh =>
      React.UpdateWithSideEffects(
        {...state, data: None, loading: true},
        (
          ({send}) => {
            Storage.clearSubscriptionList();
            fetchData(~send, ~refresh=true, ());
          }
        ),
      )
    | Loading => React.Update({...state, loading: true})
    | OpenActionSheet(articleIdx) =>
      React.Update({...state, articleIdx: Some(articleIdx)})
    | HideActionSheet => React.Update({...state, articleIdx: None})
    | Loaded(data) =>
      React.Update({...state, loading: false, data: Some(data)})
    | UpdateArticle(idx, a) =>
      switch (state.data) {
      | None => React.NoUpdate
      | Some(data) =>
        data.list[idx] = a;
        Storage.clearFavoriteList();
        React.Update({...state, data: Some(data), articleIdx: None});
      }
    },

  willUpdate: _ => listViewTime.clean(),

  didMount: self => {
    fetchData(~send=self.send, ());

    self.onUnmount(PubSub.subscribe(RefreshList, _ => self.send(Refresh)));
  },

  render: self => {
    let {articleIdx, data} = self.state;

    let article =
      switch (articleIdx, data) {
      | (Some(idx), Some(data)) => Some(data.list[idx])
      | _ => None
      };

    let onClickActionSheet = actionIdx =>
      switch (articleIdx) {
      | Some(articleIdx) =>
        self.send(HideActionSheet);
        onClickActionSheet(
          actionIdx,
          ~article,
          ~toast=t => self.send(Toast(t)),
          ~afterAction=
            article => self.send(UpdateArticle(articleIdx, article)),
        );
      | None => ()
      };

    <>
      <ListView
        id="subscription-list"
        key="subscription-list"
        className="subscription-list"
        data={self.state.data}
        loading={self.state.loading}
        renderEmpty
        renderItem={renderArticle(~send=self.send)}
        onEndReached={fetchData(~send=self.send, ~refresh=true, ~data)}
      />
      <ActionSheet
        visible={articleIdx != None}
        data={actionSheetData(article)}
        onClick=onClickActionSheet
        onCancel={_e => self.send(HideActionSheet)}
      />
      <Toast visible={self.state.toastVisible}>
        ...{React.string(self.state.toastText)}
      </Toast>
    </>;
  },
};