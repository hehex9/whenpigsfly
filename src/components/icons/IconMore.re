let component = React.statelessComponent("IconMore");

let make = _children => {
  ...component,

  render: _self =>
    <svg className="icon ion-ios-more" viewBox="0 0 512 512">
      <g>
        <path
          d="M255.8,218c-21,0-38,17-38,38s17,38,38,38c21,0,38-17,38-38S276.8,218,255.8,218L255.8,218z"
        />
        <path
          d="M102,218c-21,0-38,17-38,38s17,38,38,38c21,0,38-17,38-38S123,218,102,218L102,218z"
        />
        <path
          d="M410,218c-21,0-38,17-38,38s17,38,38,38c21,0,38-17,38-38S431,218,410,218L410,218z"
        />
      </g>
    </svg>,
};