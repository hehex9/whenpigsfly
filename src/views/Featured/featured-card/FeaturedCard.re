open Response;

Import.style("./style.styl");

let component = React.statelessComponent("ItemCard");

let make = (~data: Shape.featured, _children) => {
  ...component,

  render: _self =>
    if (data.type_ == 0) {
      let cover = data.cover;
      <div
        className="featured-card" onClick={_e => Wx.toArticle(data.targetId)}>
        <div
          className="featured-cover"
          style={
            ReactDOMRe.Style.make(~backgroundImage={j|url($cover)|j}, ())
          }
        />
        <div className="featured-card-info">
          <p> {React.string(data.title)} </p>
          <span> {React.string(data.author)} </span>
        </div>
      </div>;
    } else {
      React.null;
    },
};