Import.style("./style.styl");

open Response;

type article = Shape.articleDetail;

type state = {
  article: option(article),
  loading: bool,
};

type action =
  | UpdateArticle(article)
  | Loaded(option(article));

let component = React.reducerComponent("ArticlePage");

let onAction = (_e, ~article as a: Shape.articleDetail, ~touch) => {
  a.isFavorited ?
    Api.removeFavorite(a.id, _ => touch({...a, isFavorited: false})) :
    Api.createFavorite(a.id, _ => touch({...a, isFavorited: true}));
  Storage.clearFavoriteList();
};

let renderAction = (article: Shape.articleDetail, send) =>
  <div
    className={
      "article-fav-action" ++ (article.isFavorited ? " favorited" : "")
    }
    onClick={onAction(~article, ~touch=a => send(UpdateArticle(a)))}>
    <IconHeart />
  </div>;

let make = _children => {
  ...component,

  initialState: () => {article: None, loading: true},

  reducer: (action, state) =>
    switch (action) {
    | UpdateArticle(article) =>
      React.Update({...state, article: Some(article)})
    | Loaded(article) => React.Update({loading: false, article})
    },

  didMount: self => {
    let id = Util.Url.getQuery("id");
    switch (id) {
    | "" => self.send(Loaded(None))
    | id => Api.fetchArticleContent(id, res => self.send(Loaded(Some(res))))
    };
  },

  render: self =>
    switch (self.state.loading, self.state.article) {
    | (true, _)
    | (_, None) => <Spinner loading=true />
    | (_, Some(article)) =>
      <>
        <h1 className="article-title"> {React.string(article.title)} </h1>
        <p className="article-author">
          {
            switch (String.length(article.author)) {
            | 0 => React.null
            | _ =>
              <span className="article-author-editor">
                {React.string(article.author)}
              </span>
            }
          }
          <span
            onClick=(_e => Wx.toWechat(article.wechat.id))
            className="article-author-wechat">
            {React.string(article.wechat.name)}
          </span>
          <span className="article-time">
            {React.string(Text.showDate(article.createdAt))}
          </span>
        </p>
        <div id="html" dangerouslySetInnerHTML={"__html": article.content} />
        {
          Account.isAuthenticated() ?
            renderAction(article, self.send) : React.null
        }
      </>
    },
};