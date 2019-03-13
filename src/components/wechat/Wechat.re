Import.style("./style.styl");

let component = React.statelessComponent("Wechat");

let toWechat = (id, _e) => Wx.toWechat(id);

let make = (~data: Response.Shape.wechat, ~onUpdate, _children) => {
  ...component,

  render: _self =>
    <div className="wechat-card hairline hairline-bottom">
      <img src={data.avatar} alt="" onClick={toWechat(data.id)} />
      <div className="wechat-card-text">
        <strong onClick={toWechat(data.id)}>
          {React.string(data.name)}
        </strong>
        <p> {React.string(Text.trim(data.description))} </p>
      </div>
      <WechatFoButton data onUpdate />
    </div>,
};