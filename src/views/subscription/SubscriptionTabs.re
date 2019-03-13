open SubscriptionTypes;

let component = React.statelessComponent("SubscriptionTabs");

let renderBottomLine = tab => {
  let cls =
    switch (tab) {
    | Left => "bottom-line-left"
    | Right => "bottom-line-right"
    };

  <span className={"subscription-tabs-bottom-line " ++ cls} />;
};

let className = active =>
  "subscription-tabs-item " ++ (active ? "active" : "");

let make = (~tab, ~onChangeTab, _children) => {
  ...component,

  render: _self =>
    <div className="subscription-tabs hairline hairline-bottom">
      <div
        className={className(tab == Left)} onClick={_e => onChangeTab(Left)}>
        <IconBookmark />
      </div>
      <div
        className={className(tab == Right)}
        onClick={_e => onChangeTab(Right)}>
        <IconAdd />
      </div>
      {renderBottomLine(tab)}
    </div>,
};