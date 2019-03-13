let component = React.statelessComponent("IconAdd");

let make = _children => {
  ...component,
  render: _self =>
    <svg className="icon ion-ios-add" viewBox="0 0 512 512">
      <g>
        <path
          d="M368.5,240H272v-96.5c0-8.8-7.2-16-16-16c-8.8,0-16,7.2-16,16V240h-96.5c-8.8,0-16,7.2-16,16c0,4.4,1.8,8.4,4.7,11.3
		c2.9,2.9,6.9,4.7,11.3,4.7H240v96.5c0,4.4,1.8,8.4,4.7,11.3c2.9,2.9,6.9,4.7,11.3,4.7c8.8,0,16-7.2,16-16V272h96.5
		c8.8,0,16-7.2,16-16C384.5,247.2,377.3,240,368.5,240z"
        />
      </g>
    </svg>,
};