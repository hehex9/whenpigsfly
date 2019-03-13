Import.style("./style.styl");

open SubscriptionTypes;

type state = {tab: subTab};

type action =
  | ChangeTab(subTab);

let component = React.reducerComponent("Subscription");

let make = _children => {
  ...component,

  initialState: () => {tab: Account.isAuthenticated() ? Left : Right},

  reducer: (action, state) =>
    switch (action) {
    | ChangeTab(tab) =>
      if (tab == state.tab) {
        PubSub.publish(ScrollToTop, ());
        React.NoUpdate;
      } else {
        React.Update({tab: tab});
      }
    },

  render: self => {
    let isAuthenticated = Account.isAuthenticated();

    <div className="subscription-container">
      {
        isAuthenticated ?
          <>
            <SubscriptionTabs
              tab={self.state.tab}
              onChangeTab={tab => self.send(ChangeTab(tab))}
            />
            {
              switch (self.state.tab) {
              | Left => <SubscriptionList />
              | Right => <CategoryList />
              }
            }
          </> :
          <CategoryList />
      }
    </div>;
  },
};