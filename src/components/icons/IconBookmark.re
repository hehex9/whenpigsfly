let component = React.statelessComponent("IconBookmark");

let make = _children => {
  ...component,
  render: _self =>
    <svg className="icon ion-ios-bookmark" viewBox="0 0 512 512">
      <path
        d="M128,80v380c0,3.3,3.8,5.2,6.4,3.2l116.8-92c2.9-2.1,6.8-2.1,9.6,0l116.8,92c2.6,2,6.4,0.1,6.4-3.2V80c0-17.7-14.3-32-32-32
      H160C142.3,48,128,62.3,128,80z"
      />
    </svg>,
};