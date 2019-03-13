Import.style("./style.styl");

let component = React.statelessComponent("Tabbar");

let tabItemClass = (active: bool): string =>
  active ? "tabbar-item tabbar-item--active" : "tabbar-item";

let changeTab = (currentTab: Router.route, nextTab: Router.route) =>
  switch (nextTab, currentTab, Account.isAuthenticated()) {
  | (Favorites, _, false) => Wx.toAuth()
  | (a, b, _) when a == b => ()
  | _ => Router.push(nextTab)
  };

let renderTabIcon = (tab, icon, ~currentTab) => {
  let isCurrentTab = currentTab == tab;
  let onClick =
    isCurrentTab ?
      _e => PubSub.publish(RefreshList, ()) :
      (_e => changeTab(currentTab, tab));

  <div className={tabItemClass(isCurrentTab)} onClick>
    {isCurrentTab ? <IconRefresh /> : icon}
  </div>;
};

let make = (~route, _children) => {
  ...component,

  render: _self => {
    let renderIcon = renderTabIcon(~currentTab=route);

    <div className="tabbar hairline hairline-top">
      {renderIcon(Featured, <IconIceCream />)}
      {renderIcon(Subscription, <IconBookmark />)}
      {renderIcon(Favorites, <IconHeart />)}
    </div>;
  },
};