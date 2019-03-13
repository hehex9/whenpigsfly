Import.style("./style.styl");

let component = React.statelessComponent("ArticleItem");

let make =
    (
      ~className="",
      ~data: Response.Shape.article,
      ~onAction,
      ~showTime=false,
      ~showAction=true,
      ~showHeart=false,
      _children,
    ) => {
  ...component,

  render: _self => {
    let cover = data.cover;

    <div className={"article-item " ++ className}>
      <div
        className="article-item-inner" onClick={_e => Wx.toArticle(data.id)}>
        <div className="article-item-cover-wrapper">
          <div
            className="article-item-cover"
            style={
              ReactDOMRe.Style.make(~backgroundImage={j|url($cover)|j}, ())
            }
          />
        </div>
        <div className="article-item-title">
          {React.string(Text.trim(data.title, ~count=28))}
        </div>
        <div className="article-item-name">
          {
            React.string(
              showTime ? Text.showDate(data.createdAt) : data.wechat.name,
            )
          }
        </div>
      </div>
      <div className="article-actions">
        {
          showHeart ?
            <span
              className={
                "article-action-icon "
                ++ (data.isFavorited ? "" : "unfavorited")
              }
              onClick=onAction>
              <IconHeart />
            </span> :
            React.null
        }
        {
          showAction ?
            <span className="article-action-icon" onClick=onAction>
              {data.isFavorited ? <IconMoreAndMore /> : <IconMore />}
            </span> :
            React.null
        }
      </div>
    </div>;
  },
};