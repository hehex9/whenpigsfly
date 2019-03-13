Import.style("./button.styl");

open Response;

let component = React.statelessComponent("WechatFollowButton");

let onClick = (data: Response.Shape.wechat, onUpdate) =>
  switch (Account.isAuthenticated(), data.isSubscribed) {
  | (false, _) => Wx.toAuth()
  | (true, true) =>
    Api.removeSubscription(data.id, _ =>
      onUpdate({...data, isSubscribed: false})
    )
  | (true, false) =>
    Api.createSubscription(data.id, _ =>
      onUpdate({...data, isSubscribed: true})
    )
  };

let actionCls = isSubscribed =>
  isSubscribed ? "wechat-card-action wechat-subscribed" : "wechat-card-action";

let make = (~data: Shape.wechat, ~onUpdate, _children) => {
  ...component,
  render: _self =>
    <button
      className={actionCls(data.isSubscribed)}
      onClick={_e => onClick(data, onUpdate)}>
      {React.string(data.isSubscribed ? {js|取消|js} : {js|订阅|js})}
    </button>,
};