Import.style("./style.styl");

let make = _children => {
  ...React.statelessComponent("Not Found"),

  render: _self =>
    <h1 className="notfound-title"> {React.string("404")} </h1>,
};