type state = {
  route: Router.route,
  loaded: bool,
};

type action =
  | DidMount
  | ChangeRoute(Router.route);

let component = React.reducerComponent("App");

let renderTabViews = (r: Router.route) =>
  <div
    className={
      "container"
      ++ (Account.isAuthenticated() ? " authenticated" : "")
      ++ (Util.os() == IOS ? " os-ios" : " os-android")
    }>
    {
      switch (r) {
      | Featured => <FeaturedView />
      | Subscription => <SubscriptionView />
      | Favorites => <FavoritesView />
      | _ => <FeaturedView />
      }
    }
    <Tabs route=r />
  </div>;

let make = _children => {
  ...component,

  initialState: () => {route: Router.currentRoute(), loaded: false},

  reducer: (action, state) =>
    switch (action) {
    | DidMount => React.Update({...state, loaded: true})
    | ChangeRoute(route) => React.Update({...state, route})
    },

  didMount: self => {
    let watchId =
      Router.watchUrl(url =>
        self.send(ChangeRoute(url |> Router.mapUrlToRoute))
      );
    self.onUnmount(() => Router.unwatchUrl(watchId));
    self.send(DidMount);
  },

  render: ({React.state}) =>
    switch (state.route, state.loaded) {
    | (_, false) => React.null
    | (Featured | Subscription | Favorites, true) =>
      state.route |> renderTabViews
    | _ => <NotFoundView />
    },
};